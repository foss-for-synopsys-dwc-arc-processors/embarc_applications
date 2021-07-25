package com.example.aipd

import android.content.Context
import android.content.Intent
import android.graphics.drawable.TransitionDrawable
import android.os.Bundle
import android.view.animation.AnimationUtils
import android.widget.Button
import android.widget.ImageView
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity

class ProfileActivity: AppCompatActivity()  {
    override fun onCreate(savedInstanceState: Bundle?){
        super.onCreate(savedInstanceState)
        setContentView(R.layout.profile)


        val mainMenuButton = findViewById<Button>(R.id.mainMenu)
        val logOutButton = findViewById<Button>(R.id.logOut)

        val preferences = getSharedPreferences("usrName", Context.MODE_PRIVATE)
        val user = preferences.getString( "User", "Doesn't exist.")
        val profileText = findViewById<TextView>(R.id.profileText)

        profileText.text = user

        mainMenuButton.setOnClickListener {
            startActivity(Intent(this, RecordingActivity::class.java))
        }

        logOutButton.setOnClickListener {
            startActivity(Intent(this, MainActivity::class.java))
        }


//            test
//        val roun dButton = findViewById<Button>(R.id.tester)
//        roundButton.getba
//        val transitiondrawable = TransitionDrawable.


        val rtl = AnimationUtils.loadAnimation(this, R.anim.ltr);
        val shoe = findViewById<ImageView>(R.id.shoe)

        shoe.startAnimation(rtl)

    }






}