package com.example.aipd

import android.content.Intent
import android.os.Bundle
import android.widget.Button
import android.widget.ImageButton
import android.widget.ImageView
import androidx.appcompat.app.AppCompatActivity

class ShowResultActivity: AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.show_result)
        val profileButton = findViewById<ImageButton>(R.id.profileButon2)
        val backButton = findViewById<Button>(R.id.backButton)
        val pointer = findViewById<ImageView>(R.id.pointer)

        backButton.setOnClickListener {
            startActivity(Intent(this, RecordingActivity::class.java))
        }


        val rotateButton = findViewById<Button>(R.id.push)
        val score = 87


        val deg =  when {
            score < 50 ->  (score/ 50 * 30F)   //normal
            score < 60 -> (score - 50) *3 + 30F  //mild
            score < 80 ->  (score - 60)/20*30 + 60F  //moderate
            score < 90 ->  (score - 80) *3 + 90F  //severe
            score < 95 ->  (score - 90) *6 + 120F  //very severe
            else ->  (score - 95) *6 + 150F  //extreme
        }

        /*
        if( score < 50){
            //normal
            rotateDeg = (score/ 50 * 30F)
        }
        else if( score < 60 ){
            //mild
            rotateDeg = (score - 50) *3 + 30F
        }
        else if( score < 80 ){
            //moderate
            rotateDeg = (score - 60)/20*30 + 60F
        }
        else if( score < 90 ){
            //severe
            rotateDeg = (score - 80) *3 + 90F
        }
        else if( score < 95 ){
            //very severe
            rotateDeg = (score - 90) *6 + 120F
        }
        else {
            //extreme
            rotateDeg = (score - 95) *6 + 150F
        }

         */


//        rotateButton.setOnClickListener {
//
//            /*
//            val rotateAnimation = RotateAnimation(
//                0f, 90f,
//                Animation.RELATIVE_TO_SELF, 0.5f,
//                Animation.RELATIVE_TO_SELF, 0.5f
//
//            )
//            rotateAnimation.duration = 1000
//
//            pointer.startAnimation(rotateAnimation)
//            pointer.rotation = 90f
//             */
//            pointer.animate().rotation(deg).duration = 1200
//
//        }
        pointer.animate().rotation(deg).duration = 2000

        profileButton.setOnClickListener {
            startActivity(Intent(this, ProfileActivity::class.java))
        }

    }
}