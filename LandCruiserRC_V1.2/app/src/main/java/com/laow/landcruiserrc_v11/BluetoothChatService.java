/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.laow.landcruiserrc_v11;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothServerSocket;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

/*
* 管理接入的设备, connectThread, connectingThread, connectedThread,
* connectFail, connectLost,
* */
public class BluetoothChatService {
    /*调试*/
    private static final String TAG = "BluetoothChatService";
    private static final boolean D = true;
    private static final String NAME = "BluetoothChat";
    /*特定传输类型的UUID, 适合发送字符数据*/
    private static final UUID MY_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    public static StringBuffer hexString = new StringBuffer();
    private final BluetoothAdapter mAdapter;
    private final Handler mHandler;
    private AcceptThread mAcceptThread;
    private ConnectThread mConnectThread;
    private ConnectedThread mConnectedThread;
    private int mState;
    /*常数，指示当前的连接状态*/
    public static final int STATE_NONE = 0;       // 没有传入的连接
    public static final int STATE_LISTEN = 1;     // 侦听传入的连接
    public static final int STATE_CONNECTING = 2; // 开始传出的连接
    public static final int STATE_CONNECTED = 3;  // 连接到设备
    public static boolean bRun = true;

    /*找到adapter, 初始化handle*/
    public BluetoothChatService(Context context, Handler handler)
    {
        mAdapter = BluetoothAdapter.getDefaultAdapter();
        mState = STATE_NONE;
        mHandler = handler;
    }

    /*从全局消息池里获取消息, 发送给目标handler*/
    private synchronized void setState(int state) {
        if (D) {
            Log.d(TAG, "setState() " + mState + " -> " + state);
        }
        mState = state;
        /* 给新状态的处理程序，界面活性可以更新  从全局消息池里获取消息, 发送给目标Handler处理*/
        mHandler.obtainMessage(MainActivity.MESSAGE_STATE_CHANGE, state, -1).sendToTarget();
    }

    /*获取状态*/
    public synchronized int getState() {
        return mState;
    }

    /* 1 ConnectThread.start()*/
    public synchronized void connect(BluetoothDevice device) {
        if (D) Log.d(TAG, "connect to: " + device);
        /*取消任何线程试图建立连接*/
        if (mState == STATE_CONNECTING) {
            if (mConnectThread != null) {
                mConnectThread.cancel();
                mConnectThread = null;
            }
        }
        /*取消任何线程正在运行的连接*/
        if (mConnectedThread != null) {
            mConnectedThread.cancel();
            mConnectedThread = null;
        }
        /*启动线程连接的设备*/
        mConnectThread = new ConnectThread(device);
        mConnectThread.start();
        setState(STATE_CONNECTING);
    }

    /* 2 电缆替代协议指定的uuid信道服务: 利用指定的UUID创建信道, 即产生BluetoothSocket*/
    private class ConnectThread extends Thread {

        private final BluetoothSocket mmSocket;
        private final BluetoothDevice mmDevice;

        public ConnectThread(BluetoothDevice device) {
            mmDevice = device;
            BluetoothSocket tmp = null;

            try {
                tmp = device.createRfcommSocketToServiceRecord(MY_UUID);
            } catch (IOException e) {
                Log.e(TAG, "create() failed", e);
            }
            mmSocket = tmp;
        }

        public void run() {
            Log.i(TAG, "BEGIN mConnectThread");
            setName("ConnectThread");
            /*总是取消的发现，因为它会减缓连接*/
            mAdapter.cancelDiscovery();
            try {
                /*这是一个阻塞调用和将只返回一个成功的连接或例外*/
                mmSocket.connect();
            } catch (IOException e) {
                connectionFailed();
                try {
                    mmSocket.close();
                } catch (IOException e2) {
                    Log.e(TAG, "unable to close() socket during connection failure", e2);
                }
                /*启动服务在重新启动聆听模式*/
                BluetoothChatService.this.start();
                return;
            }
            /*因为我们所做的connectthread复位*/
            synchronized (BluetoothChatService.this) {
                mConnectThread = null;
            }
            /*启动连接线程*/
            connected(mmSocket, mmDevice);
        }

        public void cancel() {
            try {
                mmSocket.close();
            } catch (IOException e) {
                Log.e(TAG, "close() of connect socket failed", e);
            }
        }
    }

    /* 3' AcceptThread:start()*/
    public synchronized void start() {
        if (D) Log.d(TAG, "start");
        /*取消任何线程试图建立连接*/
        if (mConnectThread != null) {
            mConnectThread.cancel();
            mConnectThread = null;
        }
        /*取消任何线程正在运行的连接*/
        if (mConnectedThread != null) {
            mConnectedThread.cancel();
            mConnectedThread = null;
        }
        /*启动线程来听一个bluetoothserversocket*/
        if (mAcceptThread == null) {
            mAcceptThread = new AcceptThread();
            mAcceptThread.start();
        }
        setState(STATE_LISTEN);
    }

    /* 3'' 监听信道*/
    private class AcceptThread extends Thread {
        /*本地服务器套接字*/
        private final BluetoothServerSocket mmServerSocket;

        public AcceptThread() {
            BluetoothServerSocket tmp = null;
            /*创建一个新的侦听服务器套接字*/
            try {
                tmp = mAdapter.listenUsingRfcommWithServiceRecord(NAME, MY_UUID);
            } catch (IOException e) {
                Log.e(TAG, "listen() failed", e);
            }
            mmServerSocket = tmp;
        }

        public void run() {
            if (D) Log.d(TAG, "BEGIN mAcceptThread" + this);
            setName("AcceptThread");
            BluetoothSocket socket = null;

            /*监听服务器套接字如果我们没有连接*/
            while (mState != STATE_CONNECTED) {
                try {
                    /*这是一个阻塞调用和将只返回一个成功的连接或例外*/
                    socket = mmServerSocket.accept();
                } catch (IOException e) {
                    Log.e(TAG, "accept() failed", e);
                    break;
                }
                /*如果连接被接受*/
                if (socket != null) {
                    synchronized (BluetoothChatService.this) {
                        switch (mState) {
                            case STATE_LISTEN:
                            case STATE_CONNECTING:
                                /*正常情况,启动连接*/
                                connected(socket, socket.getRemoteDevice());
                                break;
                            case STATE_NONE:
                            case STATE_CONNECTED:
                                /*没有准备或已连接*/
                                try {
                                    socket.close();
                                } catch (IOException e) {
                                    Log.e(TAG, "Could not close unwanted socket", e);
                                }
                                break;
                        }
                    }
                }
            }
            if (D) Log.i(TAG, "END mAcceptThread");
        }

        public void cancel() {
            if (D) Log.d(TAG, "cancel " + this);
            try {
                mmServerSocket.close();
            } catch (IOException e) {
                Log.e(TAG, "close() of server failed", e);
            }
        }
    }

    /* 4 connectedThread.start()*/
    @SuppressWarnings("unused")
    public synchronized void connected(BluetoothSocket socket, BluetoothDevice device) {
        if (D) Log.d(TAG, "connected");
        /*取消线程完成连接*/
        if (mConnectThread != null) {
            mConnectThread.cancel();
            mConnectThread = null;
        }
        /*取消任何线程正在运行的连接*/
        if (mConnectedThread != null) {
            mConnectedThread.cancel();
            mConnectedThread = null;
        }
        /*取消接受线程只因为我们要连接到一个设备*/
        if (mAcceptThread != null) {
            mAcceptThread.cancel();
            mAcceptThread = null;
        }
        /*启动线程管理连接和传输*/
        mConnectedThread = new ConnectedThread(socket);
        mConnectedThread.start();
        /*把名字的连接设备到Activity*/
        Message msg = mHandler.obtainMessage(MainActivity.MESSAGE_DEVICE_NAME);
        Bundle bundle = new Bundle();
        bundle.putString(MainActivity.DEVICE_NAME, device.getName());
        msg.setData(bundle);
        mHandler.sendMessage(msg);
        setState(STATE_CONNECTED);
    }

    /* 5 处理连接后所有的输入输出动作*/
    private class ConnectedThread extends Thread {
        private final BluetoothSocket mmSocket;
        private final InputStream mmInStream;
        private final OutputStream mmOutStream;

        public ConnectedThread(BluetoothSocket socket) {
            Log.d(TAG, "create ConnectedThread");
            mmSocket = socket;
            InputStream tmpIn = null;
            OutputStream tmpOut = null;

            /*获得bluetoothsocket输入输出流*/
            try {
                tmpIn = socket.getInputStream();
                tmpOut = socket.getOutputStream();
            } catch (IOException e) {
                Log.e(TAG, "没有创建临时sockets", e);
            }

            mmInStream = tmpIn;
            mmOutStream = tmpOut;

        }

        public void run() {
            Log.i(TAG, "BEGIN mConnectedThread");
            byte[] buffer = new byte[1024];
            int bytes;

            while (true) {
                try {
                    /*读取输入流*/
                    bytes = mmInStream.read(buffer);
                    /*发送获得的字节的用户界面*/
                    mHandler.obtainMessage(MainActivity.MESSAGE_READ, bytes, -1, buffer)
                            .sendToTarget();
                } catch (IOException e) {
                    Log.e(TAG, "disconnected", e);
                    connectionLost();
                    break;
                }
            }
        }

        /*这是一个字节流*/
        public void write(byte[] buffer) {
            try {

                mmOutStream.write(buffer);
                /*分享发送的信息到Activity*/
                mHandler.obtainMessage(MainActivity.MESSAGE_WRITE, -1, -1, buffer).sendToTarget();
            } catch (IOException e) {
                Log.e(TAG, "Exception during write", e);
            }
        }

        public void cancel() {
            try {
                mmSocket.close();
            } catch (IOException e) {
                Log.e(TAG, "close() of connect socket failed", e);
            }
        }
    }

    /* 6 ConnectedThread.write(data)*/
    public void write(byte[] out) {
        /*创建临时对象*/
        ConnectedThread r;
        /*同步副本的connectedthread*/
        synchronized (this) {
            if (mState != STATE_CONNECTED) return;
            r = mConnectedThread;
        }
        r.write(out);
    }

    /*Indicate that the connection attempt failed and notify the UI Activity.*/
    private void connectionFailed() {
        setState(STATE_LISTEN);
        /*发送失败的信息带回活动*/
        Message msg = mHandler.obtainMessage(MainActivity.MESSAGE_TOAST);
        Bundle bundle = new Bundle();
        bundle.putString(MainActivity.TOAST, "无法连接装置");
        msg.setData(bundle);
        mHandler.sendMessage(msg);
    }

    /*Indicate that the connection was lost and notify the UI Activity.*/
    private void connectionLost() {
        setState(STATE_LISTEN);
        /*发送丢失的信息带回Activity*/
        Message msg = mHandler.obtainMessage(MainActivity.MESSAGE_TOAST);
        Bundle bundle = new Bundle();
        bundle.putString(MainActivity.TOAST, "装置连接丢失");
        msg.setData(bundle);
        mHandler.sendMessage(msg);
    }

    /*停止所有的线程*/
    public synchronized void stop() {
        if (D) Log.d(TAG, "stop");
        if (mConnectThread != null) {
            mConnectThread.cancel();
            mConnectThread = null;
        }
        if (mConnectedThread != null) {
            mConnectedThread.cancel();
            mConnectedThread = null;
        }
        if (mAcceptThread != null) {
            mAcceptThread.cancel();
            mAcceptThread = null;
        }
        setState(STATE_NONE);
    }
}
