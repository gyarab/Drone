package com.example.minion.dronebtcontroller

import android.util.Log
import java.io.OutputStream

class BluetoothEnginePower(private val outputStream: OutputStream, private val value:Int, private val motorNumber:Int):Thread() {
    override fun run() {
        var message = "$motorNumber="
        message+= "${value+1000}"
        Log.i("App",message)
        for (char in message)outputStream.write(char.toInt())
        super.run()
    }
}