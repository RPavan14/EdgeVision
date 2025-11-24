package com.edgevision.pro

import android.Manifest
import android.content.pm.PackageManager
import android.graphics.SurfaceTexture
import android.opengl.GLSurfaceView
import android.os.Bundle
import android.util.Log
import android.view.TextureView
import android.widget.Button
import android.widget.TextView
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import com.edgevision.pro.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity(), TextureView.SurfaceTextureListener {
    
    private lateinit var binding: ActivityMainBinding
    private lateinit var cameraManager: CameraManager
    private lateinit var glRenderer: GLRenderer
    private var isProcessingEnabled = true
    private var fpsCounter = FPSCounter()
    
    companion object {
        private const val TAG = "EdgeVision"
        private const val CAMERA_PERMISSION_REQUEST = 100
        
        init {
            if (!System.loadLibrary("edgevision")) {
                Log.e(TAG, "Failed to load native library")
            }
        }
    }
    
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        
        setupUI()
        checkCameraPermission()
    }
    
    private fun setupUI() {
        binding.textureView.surfaceTextureListener = this
        
        binding.toggleButton.setOnClickListener {
            isProcessingEnabled = !isProcessingEnabled
            val mode = if (isProcessingEnabled) "Edge Detection" else "Raw Camera"
            binding.toggleButton.text = "Mode: $mode"
            nativeSetProcessingMode(isProcessingEnabled)
        }
        
        binding.glSurfaceView.setEGLContextClientVersion(2)
        glRenderer = GLRenderer()
        binding.glSurfaceView.setRenderer(glRenderer)
        binding.glSurfaceView.renderMode = GLSurfaceView.RENDERMODE_CONTINUOUSLY
        
        // Start FPS counter
        startFPSUpdates()
    }
    
    private fun checkCameraPermission() {
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.CAMERA) 
            != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(
                this, 
                arrayOf(Manifest.permission.CAMERA), 
                CAMERA_PERMISSION_REQUEST
            )
        } else {
            initializeCamera()
        }
    }
    
    override fun onRequestPermissionsResult(
        requestCode: Int, 
        permissions: Array<String>, 
        grantResults: IntArray
    ) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        if (requestCode == CAMERA_PERMISSION_REQUEST) {
            if (grantResults.isNotEmpty() && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                initializeCamera()
            } else {
                Toast.makeText(this, "Camera permission required", Toast.LENGTH_LONG).show()
                finish()
            }
        }
    }
    
    private fun initializeCamera() {
        cameraManager = CameraManager(this)
        cameraManager.initialize()
    }
    
    override fun onSurfaceTextureAvailable(surface: SurfaceTexture, width: Int, height: Int) {
        Log.d(TAG, "Surface texture available: ${width}x${height}")
        if (::cameraManager.isInitialized) {
            cameraManager.startCamera(surface, width, height)
        }
        nativeInitialize(width, height)
    }
    
    override fun onSurfaceTextureSizeChanged(surface: SurfaceTexture, width: Int, height: Int) {
        Log.d(TAG, "Surface texture size changed: ${width}x${height}")
        nativeUpdateSize(width, height)
    }
    
    override fun onSurfaceTextureDestroyed(surface: SurfaceTexture): Boolean {
        if (::cameraManager.isInitialized) {
            cameraManager.stopCamera()
        }
        nativeDestroy()
        return true
    }
    
    override fun onSurfaceTextureUpdated(surface: SurfaceTexture) {
        // Process frame through native code
        val frameData = getFrameData(surface)
        if (frameData != null) {
            val processedData = nativeProcessFrame(frameData, frameData.size)
            if (processedData != null) {
                glRenderer.updateTexture(processedData)
                fpsCounter.onFrame()
            }
        }
    }
    
    private fun getFrameData(surface: SurfaceTexture): ByteArray? {
        // Convert SurfaceTexture to byte array
        // This is a simplified implementation
        return ByteArray(1920 * 1080 * 4) // RGBA
    }
    
    private fun startFPSUpdates() {
        val handler = android.os.Handler(mainLooper)
        val runnable = object : Runnable {
            override fun run() {
                val fps = fpsCounter.getFPS()
                binding.fpsText.text = "FPS: ${"%.1f".format(fps)}"
                handler.postDelayed(this, 1000)
            }
        }
        handler.post(runnable)
    }
    
    override fun onDestroy() {
        super.onDestroy()
        if (::cameraManager.isInitialized) {
            cameraManager.cleanup()
        }
    }
    
    // Native method declarations
    private external fun nativeInitialize(width: Int, height: Int): Boolean
    private external fun nativeProcessFrame(data: ByteArray, size: Int): ByteArray?
    private external fun nativeSetProcessingMode(enabled: Boolean)
    private external fun nativeUpdateSize(width: Int, height: Int)
    private external fun nativeDestroy()
}
