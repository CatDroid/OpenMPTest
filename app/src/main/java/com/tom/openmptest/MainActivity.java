package com.tom.openmptest;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.tom.usingopenmp.OpenMPTest;

public class MainActivity extends AppCompatActivity {

    private final static String TAG = "MainActivity";
    private Button mBtn = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);



        mBtn = (Button)findViewById(R.id.btnStartMP);
        mBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                Log.d(TAG,"onClick");
                new Thread(){
                    @Override
                    public void run() {
                        Log.d(TAG,"run begin");
                        OpenMPTest.moreThread();
                        Log.d(TAG,"run after");
                    }
                }.start();

            }
        });

    }



}
