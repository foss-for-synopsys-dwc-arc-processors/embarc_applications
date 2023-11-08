package com.example.aipd

import android.annotation.SuppressLint
import android.content.Context
import android.content.Intent
//import android.graphics.BlendModeColorFilter
import android.graphics.Color
import android.graphics.PorterDuff
import android.graphics.drawable.Drawable
import android.graphics.drawable.GradientDrawable
import android.os.*
//import android.graphics.drawable.LayerDrawable
import android.util.Log
import android.widget.Button
import android.widget.ImageButton
//import android.widget.ImageView
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers.Default
import kotlinx.coroutines.Dispatchers.IO
import kotlinx.coroutines.Dispatchers.Main
import kotlinx.coroutines.launch
//import androidx.appcompat.widget.AppCompatButton
//import androidx.core.content.ContextCompat
import java.io.BufferedReader
import java.io.InputStreamReader
import java.io.OutputStream
import java.io.PrintWriter
import java.lang.ref.WeakReference
import java.net.Socket
import java.util.*

class RecordingActivity : AppCompatActivity(){
    private var active : Boolean = true

    private val socketThread = SocketThread()

    //private var timerLeft: Long=0
    private lateinit var timer: CountDownTimer


    private var handler: MyHandler
    init {
        val outerClass = WeakReference(this)
        handler = MyHandler(outerClass)
    }


    override fun onCreate(savedInstanceState: Bundle?){
        super.onCreate(savedInstanceState)
        setContentView(R.layout.recording)


        val policy : StrictMode.ThreadPolicy = StrictMode.ThreadPolicy.Builder().permitAll().build()
        StrictMode.setThreadPolicy(policy)


        val profileButton = findViewById<ImageButton>(R.id.profileButton)
        val startButton = findViewById<Button>(R.id.tester)
        val timerDisplay = findViewById<TextView>(R.id.textView)
        startButton.text = "Start"


        val preferences = getSharedPreferences("usrName", Context.MODE_PRIVATE)
        val user = preferences.getString( "User", "Doesn't exist.")
        val welcomeText = findViewById<TextView>(R.id.welcome)

        welcomeText.text = "Hi ! $user"




        startButton.setOnClickListener{

            if(startButton.text == "Start" || startButton.text == "Restart"){
                startButton.text = "Stop"
//                startButton.isClickable= false

                //val left1 = findViewById<Button>(R.id.ButtonL1)
                //val l1 : GradientDrawable = left1.background.mutate() as GradientDrawable
                //left1.background.setColorFilter(0xE3170D.toInt(), PorterDuff.Mode.MULTIPLY)
                //l1.setColor(0xffffffff)
                //l1.invalidateSelf()

//                val imageview : Button = findViewById<Button>(R.id.ButtonL1)
                //imageview.setImageResource(R.drawable.left_1)
//                val tempDrawable : Drawable = imageview.background
                //tempDrawable.mutate().setColorFilter(0xE3170D.toInt(), PorterDuff.Mode.MULTIPLY)
                //tempDrawable.mutate().setColorFilter(0xE3170D, PorterDuff.Mode.MULTIPLY)

//                val l1 : GradientDrawable = tempDrawable.mutate() as GradientDrawable
//                l1.setColor(Color.parseColor("#FF00FF"))
//                l1.setStroke(2, Color.parseColor("#333333"))
 //               l1.invalidateSelf()
                //imageview.setBackgroundColor(Color.parseColor("#FFFFFF"))

                //Drawable tempDrawable = getRes;
                //val l1: LayerDrawable = getResources().getDrawable(R.drawable.left_1) as LayerDrawable

                Log.d("Amy", "counter")
                //timer
                timer = object :  CountDownTimer(10000, 1000) {
                    override fun onTick(millisUntilFinished: Long) {
//                        timerLeft = millisUntilFinished / 1000
//                        if(timerLeft<10){
//                            timerDisplay.text = "00:0$timerLeft"
//                        }
//                        else{
//                            timerDisplay.text = "00:$timerLeft"
//                        }
                        timerDisplay.text = "Data Collecting..."
                        active = !active
                        //Log.d("active", active.toString())
                        //socketThread.setMyActive(active)
                    }

                    @SuppressLint("ResourceAsColor")
                    override fun onFinish() {
                        timerDisplay.text ="done!"
                        startButton.isClickable= true
                        startButton.text= "Show \n Result"
                        startButton.textSize= 15F
                        startButton.background.setColorFilter(getResources().getColor(R.color.teal_700), PorterDuff.Mode.SRC_ATOP)


                        socketThread.setMyActive(false)
                    }
                }
                timer.start()

                //client connect
                //socketThread.start()
                socketThread.setMyActive(true)
                socketThread.setHandler(handler)
                //socketThread.sendMessage("G")
                //socketThread.run()
                socketThread.start()


                active = true
//                CoroutineScope(Main).launch{
//                    client("192.168.0.178", 5009)
//                }
            }
            else if(startButton.text == "Stop"){
                startButton.text = "Restart"
                timerDisplay.text = ""
                timer.cancel()


                socketThread.setMyActive(false)
            }
            else{
                startActivity(Intent(this, ShowResultActivity::class.java))

                startButton.text = "Start"
                socketThread.setMyActive(true)
            }
        }

        profileButton.setOnClickListener {
            startActivity(Intent(this, ProfileActivity::class.java))
        }


    }



    private fun client(address : String, port : Int){
        val connection = Socket(address, port)
        val writer : OutputStream = connection.getOutputStream()
        writer.write("G".toByteArray())
        val  reader = Scanner(connection.getInputStream(), "UTF8")
        //while(active)
        while(true)
        {

            var input = ""
            if(active){
            //if(reader.hasNextLine() && active){
                active = false

                if(reader.hasNextLine()){
                    Log.d("client", "Get input")
                    input = reader.nextLine()
                    Log.d("client", input)
                }
                Log.d("client", "1")
                val ivL1 : Button = findViewById<Button>(R.id.ButtonL1)
                Log.d("client", "2")
                val tdL1 : Drawable = ivL1.background
                Log.d("client", "3")
                val l1 : GradientDrawable = tdL1.mutate() as GradientDrawable
                Log.d("client", "4")
                l1.setColor(Color.parseColor("#FF00FF"))
                Log.d("client", "5")
                l1.setStroke(2, Color.parseColor("#333333"))
                Log.d("client", "6")
                l1.invalidateSelf()
                Log.d("client", "7")
                //active = true
                Log.d("client", "8")
            }

            /*
            if(input != ""){
                input = "{#880000,#880000,#880000,#880000,#880000,#880000,#880000,#880000,#880000,#880000,#880000,#880000,#880000,#880000,#880000,#880000}"
                //input = input!!.substring(input!!.indexOf("{") + 1, input!!.indexOf("}") + 1)
                Log.d("client", input)

                val ivL1 : Button = findViewById<Button>(R.id.ButtonL1)
                val tdL1 : Drawable = ivL1.background
                val l1 : GradientDrawable = tdL1.mutate() as GradientDrawable
                l1.setColor(Color.parseColor("#AAAA00"))
                l1.setStroke(2, Color.parseColor("#333333"))
                l1.invalidateSelf()

                //handler.handleMessage(handler.obtainMessage(0, input))
            }


             */
        }
    }

}


// Declare the Handler as a static class.
class MyHandler(private val outerClass: WeakReference<RecordingActivity>) : Handler() {

    override fun handleMessage(msg: Message) {
        //    outerClass.get()?.tv_recieve?.append(msg?.obj.toString()+"\n")
        var message : String = msg.obj as String
        //super.handleMessage(msg)
        //view.onDraw(canvas, )


        var colordata : String = message.substring(message.indexOf("#"), message.indexOf(","))
        message = message.substring(message.indexOf(",") + 1, message.indexOf("}") + 1)

        Log.d("color", colordata)
        Log.d("left msg", message)

        val ivL1 : Button = outerClass.get()!!.findViewById<Button>(R.id.ButtonL1)
        val tdL1 : Drawable = ivL1.background
        val l1 : GradientDrawable = tdL1.mutate() as GradientDrawable
        l1.setColor(Color.parseColor(colordata))
        l1.setStroke(2, Color.parseColor("#333333"))
        l1.invalidateSelf()

        colordata = message.substring(message.indexOf("#"), message.indexOf(","))
        message = message.substring(message.indexOf(",") + 1, message.indexOf("}") + 1)

        Log.d("color", colordata)
        Log.d("left msg", message)

        val ivL2 : Button = outerClass.get()!!.findViewById<Button>(R.id.ButtonL2)
        val tdL2 : Drawable = ivL2.background
        val l2 : GradientDrawable = tdL2.mutate() as GradientDrawable
        l2.setColor(Color.parseColor(colordata))
        l2.setStroke(2, Color.parseColor("#333333"))
        l2.invalidateSelf()

        colordata = message.substring(message.indexOf("#"), message.indexOf(","))
        message = message.substring(message.indexOf(",") + 1, message.indexOf("}") + 1)

        Log.d("color", colordata)
        Log.d("left msg", message)

        val ivL3 : Button = outerClass.get()!!.findViewById<Button>(R.id.ButtonL3)
        val tdL3 : Drawable = ivL3.background
        val l3 : GradientDrawable = tdL3.mutate() as GradientDrawable
        l3.setColor(Color.parseColor(colordata))
        l3.setStroke(2, Color.parseColor("#333333"))
        l3.invalidateSelf()

        colordata = message.substring(message.indexOf("#"), message.indexOf(","))
        message = message.substring(message.indexOf(",") + 1, message.indexOf("}") + 1)

        Log.d("color", colordata)
        Log.d("left msg", message)

        val ivL4 : Button = outerClass.get()!!.findViewById<Button>(R.id.ButtonL4)
        val tdL4 : Drawable = ivL4.background
        val l4 : GradientDrawable = tdL4.mutate() as GradientDrawable
        l4.setColor(Color.parseColor(colordata))
        l4.setStroke(2, Color.parseColor("#333333"))
        l4.invalidateSelf()

        colordata = message.substring(message.indexOf("#"), message.indexOf(","))
        message = message.substring(message.indexOf(",") + 1, message.indexOf("}") + 1)

        Log.d("color", colordata)
        Log.d("left msg", message)

        val ivL5 : Button = outerClass.get()!!.findViewById<Button>(R.id.ButtonL5)
        val tdL5 : Drawable = ivL5.background
        val l5 : GradientDrawable = tdL5.mutate() as GradientDrawable
        l5.setColor(Color.parseColor(colordata))
        l5.setStroke(2, Color.parseColor("#333333"))
        l5.invalidateSelf()

        colordata = message.substring(message.indexOf("#"), message.indexOf(","))
        message = message.substring(message.indexOf(",") + 1, message.indexOf("}") + 1)

        Log.d("color", colordata)
        Log.d("left msg", message)

        val ivL6 : Button = outerClass.get()!!.findViewById<Button>(R.id.ButtonL6)
        val tdL6 : Drawable = ivL6.background
        val l6 : GradientDrawable = tdL6.mutate() as GradientDrawable
        l6.setColor(Color.parseColor(colordata))
        l6.setStroke(2, Color.parseColor("#333333"))
        l6.invalidateSelf()

        colordata = message.substring(message.indexOf("#"), message.indexOf(","))
        message = message.substring(message.indexOf(",") + 1, message.indexOf("}") + 1)

        Log.d("color", colordata)
        Log.d("left msg", message)

        val ivL7 : Button = outerClass.get()!!.findViewById<Button>(R.id.ButtonL7)
        val tdL7 : Drawable = ivL7.background
        val l7 : GradientDrawable = tdL7.mutate() as GradientDrawable
        l7.setColor(Color.parseColor(colordata))
        l7.setStroke(2, Color.parseColor("#333333"))
        l7.invalidateSelf()

        colordata = message.substring(message.indexOf("#"), message.indexOf(","))
        message = message.substring(message.indexOf(",") + 1, message.indexOf("}") + 1)

        Log.d("color", colordata)
        Log.d("left msg", message)

        val ivL8 : Button = outerClass.get()!!.findViewById<Button>(R.id.ButtonL8)
        val tdL8 : Drawable = ivL8.background
        val l8 : GradientDrawable = tdL8.mutate() as GradientDrawable
        l8.setColor(Color.parseColor(colordata))
        l8.setStroke(2, Color.parseColor("#333333"))
        l8.invalidateSelf()

        Log.d("color", colordata)
        Log.d("left msg", message)

        colordata = message.substring(message.indexOf("#"), message.indexOf(","))
        message = message.substring(message.indexOf(",") + 1, message.indexOf("}") + 1)

        val ivR1 : Button = outerClass.get()!!.findViewById<Button>(R.id.ButtonR1)
        val tdR1 : Drawable = ivR1.background
        val r1 : GradientDrawable = tdR1.mutate() as GradientDrawable
        r1.setColor(Color.parseColor(colordata))
        r1.setStroke(2, Color.parseColor("#333333"))
        r1.invalidateSelf()

        Log.d("color", colordata)
        Log.d("left msg", message)

        colordata = message.substring(message.indexOf("#"), message.indexOf(","))
        message = message.substring(message.indexOf(",") + 1, message.indexOf("}") + 1)

        val ivR2 : Button = outerClass.get()!!.findViewById<Button>(R.id.ButtonR2)
        val tdR2 : Drawable = ivR2.background
        val r2 : GradientDrawable = tdR2.mutate() as GradientDrawable
        r2.setColor(Color.parseColor(colordata))
        r2.setStroke(2, Color.parseColor("#333333"))
        r2.invalidateSelf()

        colordata = message.substring(message.indexOf("#"), message.indexOf(","))
        message = message.substring(message.indexOf(",") + 1, message.indexOf("}") + 1)

        val ivR3 : Button = outerClass.get()!!.findViewById<Button>(R.id.ButtonR3)
        val tdR3 : Drawable = ivR3.background
        val r3 : GradientDrawable = tdR3.mutate() as GradientDrawable
        r3.setColor(Color.parseColor(colordata))
        r3.setStroke(2, Color.parseColor("#333333"))
        r3.invalidateSelf()

        colordata = message.substring(message.indexOf("#"), message.indexOf(","))
        message = message.substring(message.indexOf(",") + 1, message.indexOf("}") + 1)

        val ivR4 : Button = outerClass.get()!!.findViewById<Button>(R.id.ButtonR4)
        val tdR4 : Drawable = ivR4.background
        val r4 : GradientDrawable = tdR4.mutate() as GradientDrawable
        r4.setColor(Color.parseColor(colordata))
        r4.setStroke(2, Color.parseColor("#333333"))
        r4.invalidateSelf()

        colordata = message.substring(message.indexOf("#"), message.indexOf(","))
        message = message.substring(message.indexOf(",") + 1, message.indexOf("}") + 1)

        val ivR5 : Button = outerClass.get()!!.findViewById<Button>(R.id.ButtonR5)
        val tdR5 : Drawable = ivR5.background
        val r5 : GradientDrawable = tdR5.mutate() as GradientDrawable
        r5.setColor(Color.parseColor(colordata))
        r5.setStroke(2, Color.parseColor("#333333"))
        r5.invalidateSelf()

        colordata = message.substring(message.indexOf("#"), message.indexOf(","))
        message = message.substring(message.indexOf(",") + 1, message.indexOf("}") + 1)

        val ivR6 : Button = outerClass.get()!!.findViewById<Button>(R.id.ButtonR6)
        val tdR6 : Drawable = ivR6.background
        val r6 : GradientDrawable = tdR6.mutate() as GradientDrawable
        r6.setColor(Color.parseColor(colordata))
        r6.setStroke(2, Color.parseColor("#333333"))
        r6.invalidateSelf()

        colordata = message.substring(message.indexOf("#"), message.indexOf(","))
        message = message.substring(message.indexOf(",") + 1, message.indexOf("}") + 1)

        val ivR7 : Button = outerClass.get()!!.findViewById<Button>(R.id.ButtonR7)
        val tdR7 : Drawable = ivR7.background
        val r7 : GradientDrawable = tdR7.mutate() as GradientDrawable
        r7.setColor(Color.parseColor(colordata))
        r7.setStroke(2, Color.parseColor("#333333"))
        r7.invalidateSelf()

        colordata = message.substring(message.indexOf("#"), message.indexOf("}"))

        val ivR8 : Button = outerClass.get()!!.findViewById<Button>(R.id.ButtonR8)
        val tdR8 : Drawable = ivR8.background
        val r8 : GradientDrawable = tdR8.mutate() as GradientDrawable
        r8.setColor(Color.parseColor(colordata))
        r8.setStroke(2, Color.parseColor("#333333"))
        r8.invalidateSelf()


        Log.d("color", colordata)
        Log.d("left msg R8", message)

        /*
        val ivL2 : Button = outerClass.get()!!.findViewById(R.id.ButtonL2)
        val tdL2 : Drawable = ivL2.background
        val l2 : GradientDrawable = tdL2.mutate() as GradientDrawable
        l2.setColor(Color.parseColor("#FF00FF"))
        l2.setStroke(2, Color.parseColor("#333333"))
        l2.invalidateSelf()

        var colordata : String = "#880000"

        val ivL3 : Button = outerClass.get()!!.findViewById<Button>(R.id.ButtonL3)
        val tdL3 : Drawable = ivL3.background
        val l3 : GradientDrawable = tdL3.mutate() as GradientDrawable
        l3.setColor(Color.parseColor(colordata))
        l3.setStroke(2, Color.parseColor("#333333"))
        l3.invalidateSelf()
        */

    }
}



class SocketThread:Thread() {

    private lateinit var handler: MyHandler
    private var writer : PrintWriter? = null
    private var active : Boolean = false

    override fun run() {
        super.run()
        val socket = Socket("192.168.0.178", 5009)
        val input = socket.getInputStream()
        //val reader = BufferedReader(InputStreamReader(input))
        val reader = Scanner(socket.getInputStream(), "UTF8")
        var text: String
        val output = socket.getOutputStream()
        writer = PrintWriter(output, true)

        Log.d("client", "send G")
        Log.d("client", active.toString())

        writer?.println("GG")

        while (true){
            Log.d("client", "get in while")

            //text = "{#FFFFFF,#FFFFFF,#FFFFFF,#FFFFFF,#FFFFFF,#FFFFFF,#FFFFFF,#FFFFFF,#FFFFFF,#FFFFFF,#FFFFFF,#FFFFFF,#FFFFFF,#FFFFFF,#FFFFFF,#FFFFFF}"
            if (reader.hasNextLine() && this.active) {
                text = reader.nextLine()

                //將取到的String抓取{}範圍資料
                text = text!!.substring(text!!.indexOf("{") + 1, text!!.indexOf("}") + 1)
                Log.d("client", text)
                //.handleMessage(handler.obtainMessage(0, text))
                this.active = false
                handler.postDelayed({
                    handler.handleMessage(handler.obtainMessage(0, text))
                }, 50)

                this.active = true
                //從java伺服器取得值後做拆解,可使用switch做不同動作的處理
            }

        }
    }

    fun setMyActive(flag : Boolean){
        this.active = flag
    }

    fun setHandler(handler: MyHandler) {
        this.handler = handler
    }

    fun sendMessage(string: String) {
        writer?.println(string)
    }

    override fun destroy() {

    }
}

