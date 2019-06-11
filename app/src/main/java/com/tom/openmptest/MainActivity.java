package com.tom.openmptest;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.tom.usingopenmp.OpenMPTest;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;

public class MainActivity extends AppCompatActivity {

    private final static String TAG = "MainActivity";


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);




       findViewById(R.id.btnStartMP).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                Log.d(TAG,"onClick");
                new Thread("OpenMpTh"){
                    @Override
                    public void run() {
                        Log.d(TAG,"run begin");
                        OpenMPTest.moreThread();
                        Log.d(TAG,"run after");
                    }
                }.start();

                // 在上一个任务还没有完成的时候，启动下面这个任务，就会再创建一个线程组
                // 也就是两个同时执行的独立的并行区域，会分别不同的线程组执行
                // 一个线程组到达一个工作共享结构时,要么全部线程被占用 要么不被占用 (相当于一个线程组同时只只是一个并行区)
                new Thread("OpenMpTh"){
                    @Override
                    public void run() {
                        Log.d(TAG,"run begin 2");
                        OpenMPTest.moreThread();
                        Log.d(TAG,"run after 2");
                    }
                }.start();
            }
        });


        findViewById(R.id.btnTask).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                new Thread("OpenThTask"){
                    @Override
                    public void run() {
                        Log.d(TAG,"run begin");
                        OpenMPTest.doTask();
                        Log.d(TAG,"run after");
                    }
                }.start();

            }
        });


        final Bitmap bmp = BitmapFactory.decodeFile("/sdcard/colors.png");
        if(bmp != null){
            final ByteBuffer rgba = ByteBuffer.allocateDirect( bmp.getHeight()*bmp.getWidth()*4 );
            final ByteBuffer yuv = ByteBuffer.allocateDirect( bmp.getHeight()*bmp.getWidth()*3/2 );
            bmp.copyPixelsToBuffer(rgba);

            try {
                rgba.rewind();
                FileOutputStream fos = new FileOutputStream(new File("/sdcard/temp.rgba"));
                FileChannel fc = fos.getChannel();
                fc.write(rgba);
                fc.close();
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            } catch(Exception ex){
                Log.e(TAG,"open png fail " , ex );
            }


            findViewById(R.id.btnRgba2Yuv).setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    new Thread("Rgba2YuvTh"){
                        @Override
                        public void run() {
                            Log.d(TAG,"Rgba2YuvTh entry");
                            OpenMPTest.rgba2yuv420p(
                                    rgba,
                                    yuv,
                                    bmp.getWidth(),
                                    bmp.getHeight(),
                                    true);
                            Log.d(TAG,"Rgba2YuvTh done ");
                        }
                    }.start();

                }
            });

            findViewById(R.id.btnSaveYUV).setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    try {
                        FileOutputStream fos = new FileOutputStream(new File("/sdcard/temp.yuv"));
                        FileChannel fc = fos.getChannel();
                        yuv.rewind();
                        fc.write(yuv);
                        fc.close();

                        Toast.makeText(getApplicationContext(),"Save Done", Toast.LENGTH_LONG).show();

                    } catch (FileNotFoundException e) {
                        e.printStackTrace();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }


                }
            });

        }else {
            Log.e(TAG,"bmp is null");
        }


    }



}
