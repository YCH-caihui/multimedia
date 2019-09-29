package com.caihui.ffmpegsample;

public class DataClient  {

    static {
        System.loadLibrary("dataClient");
    }

    public native  void startSocketConnection(String ipAddress, int port, String userName, String password);

    public native  void stopSocketConnectin();
}
