package com.example.minion.dronebtcontroller

import android.annotation.SuppressLint
import android.annotation.TargetApi
import android.graphics.Color
import android.graphics.drawable.GradientDrawable
import android.graphics.drawable.shapes.Shape
import android.os.Build
import android.os.Bundle
import android.support.annotation.RequiresApi
import android.support.constraint.solver.Metrics
import android.support.constraint.solver.widgets.Rectangle
import android.support.v7.app.AppCompatActivity
import android.text.BoringLayout
import android.util.Base64OutputStream
import android.util.DisplayMetrics
import android.util.Log
import android.view.Gravity
import android.widget.*
import java.io.OutputStream
import kotlin.random.Random

class BluetoothControlActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_bluetooth_controller)
        val bluetoothData = getCommunicationData()
        val mainLayout = findViewById<LinearLayout>(R.id.controllerLayout)
        makeMainLayout(mainLayout, bluetoothData!!.outputStream!!)

    }

    private fun setSeekBarController(textView: TextView, outputStream: OutputStream, id: Int): SeekBar {
        val motorController = SeekBar(applicationContext)
        motorController.max = 1000
        motorController.progress = 0
        motorController.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
            override fun onProgressChanged(seekBar: SeekBar, i: Int, b: Boolean) {
                textView.text = "Výkon: ${i / 10.0}%"
                BluetoothEnginePower(outputStream, i, id).start()
            }

            override fun onStartTrackingTouch(seekBar: SeekBar) {

            }

            override fun onStopTrackingTouch(seekBar: SeekBar) {

            }
        })
        return motorController
    }

    private fun makeEngineTextView(text: String, purpose: String): TextView {
        val metrics = DisplayMetrics()
        windowManager.defaultDisplay.getMetrics(metrics)
        val textView = TextView(applicationContext)
        textView.setTextColor(Color.BLACK)
        if (purpose == "name") textView.textSize = 20F
        else textView.textSize = 17F


        textView.textAlignment = TextView.TEXT_ALIGNMENT_CENTER
        textView.width = metrics.widthPixels
        textView.text = text
        return textView
    }

    private fun makeMainLayout(mainLayout: LinearLayout, outputStream: OutputStream) {
        makeEngineCotrollerLayout("První motor: ", mainLayout, outputStream, 1)
        makeEngineCotrollerLayout("Druhý motor: ", mainLayout, outputStream, 2)
        makeEngineCotrollerLayout("Třetí motor: ", mainLayout, outputStream, 3)
        makeEngineCotrollerLayout("Čtvrtý motor: ", mainLayout, outputStream, 4)
        makeEmergencyButton(mainLayout, outputStream)

    }

    private fun makeEngineCotrollerLayout(name: String, mainLayout: LinearLayout, outputStream: OutputStream, id: Int) {
        val powerTextView = makeEngineTextView("Výkon: ${0 / 10.0}%", "")
        mainLayout.addView(makeEngineTextView(name, "name"))
        mainLayout.addView(makeSpace(50))
        mainLayout.addView(setSeekBarController(powerTextView, outputStream, id))
        mainLayout.addView(powerTextView)
        mainLayout.addView(makeSpace(70))
    }

    private fun makeSpace(height: Int): Space {
        val space = Space(applicationContext)
        space.minimumHeight = height
        return space
    }

    private fun makeEmergencyButton(mainLayout: LinearLayout, outputStream: OutputStream) {
        val linearLayout = LinearLayout(applicationContext)
        val emergencyButton = Button(applicationContext)
        val metrics = DisplayMetrics()
        emergencyButton.setTextColor(Color.WHITE)
        emergencyButton.textAlignment = TextView.TEXT_ALIGNMENT_CENTER
        windowManager.defaultDisplay.getMetrics(metrics)
        val gradientDrawable = GradientDrawable()
        gradientDrawable.setColor(Color.RED)
        gradientDrawable.setStroke(2, Color.BLACK)
        gradientDrawable.cornerRadius = 55F
        emergencyButton.background = gradientDrawable
        linearLayout.addView(emergencyButton, 3 * metrics.widthPixels / 4, 200)
        linearLayout.gravity = Gravity.CENTER
        emergencyButton.text = "オンドラと呼ばれる雌犬のその息子を救う\n(to je japonsky)"
        mainLayout.addView(makeSpace(100))
        mainLayout.addView(linearLayout)
        emergencyButton.setOnClickListener {
            val texts = arrayListOf<String>()
            texts.add("Helena Chroma ist meine größte Liebe\n (to je německy)")
            texts.add("Je t'aime, tu es le meilleur DJ\n (to je francouzky)")
            texts.add("سبعة أكثر من خمس سنوات ، كيا سبع سنوات الضمان\n (to je arabsky)")
            texts.add("オンドラと呼ばれる雌犬のその息子を救う\n(to je japonsky)")
            texts.add("אָטאַ איז טאַקע אַ גרויס טאָכטער, אָבער ער איז אַ שלאַנג\n (to je Jidišem)")
            texts.add("Zdar jak sviňa, jedeš Šalinou?\n (to je Hantecem)")
            emergencyButton.text = texts[Random.nextInt(texts.size)]
            BluetoothEnginePower(outputStream, 0, 5).start()//všechny motory
            for (i in 0 until mainLayout.childCount) {
                if (mainLayout.getChildAt(i) is SeekBar) {
                    val seekBar: SeekBar = mainLayout.getChildAt(i) as SeekBar
                    seekBar.progress = 0
                } else if (mainLayout.getChildAt(i) is TextView) {
                    val textView = mainLayout.getChildAt(i) as TextView
                    textView.text = if (textView.text.contains("Výkon")) "Výkon: ${0 / 10.0}%" else textView.text
                }
            }
        }
    }
}