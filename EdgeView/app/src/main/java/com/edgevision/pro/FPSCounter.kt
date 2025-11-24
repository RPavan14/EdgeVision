package com.edgevision.pro

class FPSCounter {
    private var frameCount = 0
    private var lastTime = System.currentTimeMillis()
    private var fps = 0.0
    
    fun onFrame() {
        frameCount++
        val currentTime = System.currentTimeMillis()
        val deltaTime = currentTime - lastTime
        
        if (deltaTime >= 1000) { // Update every second
            fps = frameCount * 1000.0 / deltaTime
            frameCount = 0
            lastTime = currentTime
        }
    }
    
    fun getFPS(): Double = fps
}
