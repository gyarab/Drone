package com.example.minion.dronebtcontroller

import android.bluetooth.BluetoothAdapter
import android.content.Intent
import android.graphics.Color
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.util.DisplayMetrics
import android.view.View
import android.widget.*
class BTChooseActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_btchoose)
        val mainLayout = findViewById<LinearLayout>(R.id.mainLayout)
        val bluetoothDevicesListView = ListView(applicationContext)
        val bluetoothAdapter = BluetoothAdapter.getDefaultAdapter()
        checkBtAdapter(bluetoothAdapter)
        val display = DisplayMetrics()
        windowManager.defaultDisplay.getMetrics(display)
        val bluetoothConnection = BluetoothConnection(bluetoothAdapter)
        val pairedDevices = bluetoothConnection.getPairedDevices()
        val pairedDeviceNames = getNames(pairedDevices)
        val arrayAdapter = ArrayAdapter<String>(applicationContext,R.layout.list_item,pairedDeviceNames)
        bluetoothDevicesListView.adapter = arrayAdapter



        val text = TextView(applicationContext)
        text.setTextColor(Color.BLACK)
        text.textAlignment = View.TEXT_ALIGNMENT_CENTER
        text.text = "Wähle eine Drohne\n (to je německy)"
        text.width = display.widthPixels
        text.textSize = 20F
        bluetoothDevicesListView.minimumWidth = display.widthPixels

        var startCommunication:StartCommunication
        var chosenDevice:PairedDevice
        bluetoothDevicesListView.setOnItemClickListener { parent, view, position, id ->
            var item = view as TextView
            chosenDevice = getPairedDevice(item.text as String,pairedDevices)!!
            startCommunication = StartCommunication(bluetoothAdapter.getRemoteDevice(chosenDevice.getBluetoothDevice().address),this)
            bluetoothAdapter.cancelDiscovery()
            startCommunication.start()
            Toast.makeText(applicationContext,"Počkej malšik",Toast.LENGTH_LONG).show()
        }

        mainLayout.addView(text)
        mainLayout.addView(bluetoothDevicesListView)
    }
     private fun getNames(array: Array<PairedDevice>):Array<String>{
        var pairedDevicesNames = arrayOf<String>()
        for (name in array){
            pairedDevicesNames += name.getName()
        }
        return pairedDevicesNames
    }
    private fun getPairedDevice(text:String, pairedDeviceNames: Array<PairedDevice>):PairedDevice?{
        for (pairedDevice in pairedDeviceNames){
            if (pairedDevice.getName() == text){
                return pairedDevice
            }
        }
        return null
    }
    private fun checkBtAdapter(bluetoothAdapter: BluetoothAdapter){
        if (bluetoothAdapter.isEnabled){
            return
        }
        bluetoothAdapter.enable()
        Toast.makeText(applicationContext,"Your Bluetooth has been turned on \n (to je anglicky)", Toast.LENGTH_LONG).show()
    }
    fun startNewActivity(){
        var intent = Intent(applicationContext,BluetoothControlActivity::class.java)
        startActivity(intent)
    }
    fun makeToast(message:String){
        Toast.makeText(applicationContext,message,Toast.LENGTH_LONG)
    }
}


