package com.example.aipd

import android.content.Context
import android.content.DialogInterface
import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.text.Editable
import android.text.TextWatcher
import android.util.Log.d
import android.view.View
import android.widget.Button
import android.widget.EditText
import androidx.appcompat.app.AlertDialog
import androidx.core.widget.addTextChangedListener
import com.google.android.material.dialog.MaterialAlertDialogBuilder
import com.google.android.material.snackbar.Snackbar

class MainActivity : AppCompatActivity() {



    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)


        val usrName = findViewById<EditText>(R.id.userName)
        val passWord = findViewById<EditText>(R.id.password)
        val signInButton = findViewById<Button>(R.id.signIn)
        val registerButton = findViewById<Button>(R.id.register)

        // enabled btn
        val inputTexts = listOf(usrName, passWord)
        for (editText in inputTexts){
            editText.addTextChangedListener(object : TextWatcher{
                override fun onTextChanged(s: CharSequence, start: Int, before: Int, count: Int) {
                    var et1 = usrName.text.toString().trim()
                    var et2 = passWord.text.toString().trim()

                    signInButton.isEnabled = et1.isNotEmpty() && et2.isNotEmpty()
                    registerButton.isEnabled = et1.isNotEmpty() && et2.isNotEmpty()
                }
                override fun beforeTextChanged(
                    s: CharSequence, start: Int, count: Int, after: Int) {
                }

                override fun afterTextChanged(
                    s: Editable) {
                }
            })
        }



        registerButton.setOnClickListener {
            val name = usrName.text.toString()
            val psswd = passWord.text.toString()
            d("Amy","Name: $name")
            d("Amy","Psswd: $psswd")

            val database = getSharedPreferences("database", Context.MODE_PRIVATE)
            database.edit().apply{
                putString( name, psswd)
            }.apply()

            val errorBuilder= AlertDialog.Builder(this)
                .setTitle("Message")
                .setMessage("Register Successfully! ")
                .setPositiveButton("OK"
                ) { dialog, which ->
                    //do nothing
                }
                .show()
        }

        signInButton.setOnClickListener {
            val name = usrName.text.toString()
            val psswd = passWord.text.toString()
            d("Amy","Name: $name")
            d("Amy","Psswd: $psswd")
            val preferences = getSharedPreferences("database",Context.MODE_PRIVATE)
            val databasePsswd = preferences.getString( name, "Doesn't exist.")
            d("Amy", "saved password is : $databasePsswd")
            if(databasePsswd == psswd){

                val database = getSharedPreferences("usrName", Context.MODE_PRIVATE)
                database.edit().apply{
                    putString( "User", name)
                }.apply()

                startActivity(Intent(this, RecordingActivity::class.java))
            }
            else if(databasePsswd == "Doesn't exist."){
                val errorBuilder= AlertDialog.Builder(this)
                    .setTitle("Invalid Account")
                    .setMessage("Your account doesn't exist. Register now!")
                    .setPositiveButton("Reister Now"
                    ) { dialog, which ->
                        //store user info
                        val database = getSharedPreferences("database", Context.MODE_PRIVATE)
                        database.edit().apply{
                            putString( name, psswd)
                        }.apply()

                        //log in
                        val database2 = getSharedPreferences("usrName", Context.MODE_PRIVATE)
                        database2.edit().apply{
                            putString( "User", name)
                        }.apply()
                        startActivity(Intent(this, RecordingActivity::class.java))
                    }

                    .setNegativeButton("Cancel"
                    ) { dialog, which ->
                    }
                    .show()
            }

            else{
                d("Amy","wrong psswd")

                val errorBuilder= AlertDialog.Builder(this)
                    .setTitle("Wrong Password")
                    .setMessage("Your password is wrong. Try again!")
                    .setPositiveButton("OK"
                    ) { dialog, which ->
                        //do nothing
                    }
                    .show()
            }


        }
    }







}