package com.example.minion.dronebtcontroller

import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothSocket
import android.util.Log
import java.io.*
import java.lang.Exception
import java.util.*

class StartCommunication(private val bluetoothDevice: BluetoothDevice, private val btChooseActivity: BTChooseActivity): Thread(){
    var bluetoothSocket:BluetoothSocket? = null
    var outputStream:OutputStream? = null
    var inputStream:InputStream? = null
    val passwordOut = "6:}k712m07"
    val passwordIn = "+9/On}2pao"

    override fun run(){
        bluetoothSocket = bluetoothDevice.createInsecureRfcommSocketToServiceRecord(UUID.fromString("00001101-0000-1000-8000-00805F9B34FB"))
        bluetoothSocket?.connect()
        outputStream = bluetoothSocket?.outputStream
        inputStream = bluetoothSocket?.inputStream
        setCommunicationData(this)
        isDroneConnected()
        super.run()
    }
    private fun isDroneConnected(){
        var receivedPassword = ""
        try {
            for (char in passwordOut) outputStream!!.write(char.toInt())
            while (inputStream!!.available()==0);
            while(inputStream!!.available()!=10);
            while (inputStream!!.available()!=0) receivedPassword+=inputStream!!.read().toChar()
            if (receivedPassword==passwordIn) btChooseActivity.startNewActivity()
            else btChooseActivity.makeToast("錯誤的設備 \n (to je mandarínsky)")
        }catch (e:Exception){
            btChooseActivity.makeToast("Něco se posralo \n (to je česky)")
        }




    }

}
