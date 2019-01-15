package com.example.minion.dronebtcontroller

import android.bluetooth.BluetoothDevice

class PairedDevice (bluetoothDevice: BluetoothDevice, name:String) {
    private val bluetoothDevice = bluetoothDevice
    private val name = name
    fun getName(): String {
        return name
    }
    fun getBluetoothDevice():BluetoothDevice{
        return bluetoothDevice
    }

}