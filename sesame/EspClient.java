package com.example.chileam.sesame;

import android.util.Log;

import java.io.IOException;
import java.io.PrintStream;
import java.net.InetAddress;
import java.net.Socket;

/**
 * Created by chileam on 2-6-2016.
 */
public class EspClient {
    private final static String IP = "192.168.178.24";
    private final static int PORT = 80;
    private static Socket Client;
    private static PrintStream Output;

    public EspClient() {
        init();
    }

    public void send(byte[] msg) {
        try {
            Output.write(msg);
        }
        catch (IOException e) {
            Log.e("TCP ", e.toString());
        }
    }

    public void init() {
        try {
            InetAddress address = InetAddress.getByName(IP);
            Client = new Socket(address, PORT);
            Output = new PrintStream(Client.getOutputStream());
        }
        catch (IOException e) {
            Log.e("TCP ", e.toString());
        }
    }

    public void close() {
        try {
            Output.close();
            Client.close();
        }
        catch (IOException e) {
            Log.e("ERROR", e.toString());
        }
    }

    public String getIP() {
        return IP;
    }

}
