package com.computingify.paddleboatcontroler;

import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;

import java.io.BufferedWriter;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;

public class Communication extends Thread {

    private static Handler mHandler;
    private final int port;
    private InetAddress serverAddress;
    private PrintWriter serverOut = null;

    public Communication(String ipAddress, int port) {
        this.port = port;
        try {
            serverAddress = InetAddress.getByName(ipAddress);
        } catch (UnknownHostException e) {
            Log.d("", "Can't convert IP address");
            e.printStackTrace();
        }
    }

    @Override
    public void run() {
        Socket socket = null;

        try {
            socket = new Socket(serverAddress, port);

            serverOut = new PrintWriter(new BufferedWriter(
                    new OutputStreamWriter(socket.getOutputStream())), true);

            serverOut.println("test#");
        } catch (IOException e) {
            e.printStackTrace();
        }

        Looper.prepare();
        mHandler = new Handler(Looper.myLooper()) {
            public void handleMessage(Message msg) {
                String str = msg.obj.toString();
                Log.d("", "message = " + str);
                serverOut.println(str);
            }
        };
        Looper.loop();

        try {// sleep the thread for 3S.
            while (true) {
                Thread.sleep(10000);
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        } finally {
            if (socket != null) {
                Log.d("", "STOP SOCKET");
                try {
                    socket.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    public Handler getHandler() {
        return mHandler;
    }
}
