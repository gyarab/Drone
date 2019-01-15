package com.example.minion.dronebtcontroller

import android.bluetooth.BluetoothAdapter

class BluetoothConnection(bluetoothAdapter: BluetoothAdapter) {
    private var bluetoothAdapter = bluetoothAdapter

    fun getPairedDevices():Array<PairedDevice>{
        var pairedDevices = bluetoothAdapter.bondedDevices
        var pairedDevicesArray = arrayOf<PairedDevice>()
         for (device in pairedDevices){
             pairedDevicesArray+=PairedDevice(device,device.name)
         }
        return pairedDevicesArray
    }
}