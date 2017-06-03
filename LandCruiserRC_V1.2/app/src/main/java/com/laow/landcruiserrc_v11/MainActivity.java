package com.laow.landcruiserrc_v11;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.os.Handler;
import android.os.Message;
import android.support.constraint.ConstraintLayout;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;

import com.laow.ui.DrawView;


public class MainActivity extends AppCompatActivity {

    private BluetoothAdapter mBluetoothAdapter = null;
    private static final int REQUEST_ENABLE_BT = 3;
    public static final int REQUEST_CONNECT_DEVICE = 1;
    public static final String TOAST = "toast";
    public static final int MESSAGE_TOAST = 5;
    public static final int MESSAGE_WRITE = 3;
    public static final int MESSAGE_READ = 2;
    public static final int MESSAGE_STATE_CHANGE = 1;
    public static final int MESSAGE_DEVICE_NAME = 4;
    public static final String DEVICE_NAME = "device_name";
    private Button scanButton;
    private Button sendButton;
    private EditText recv;
    private EditText inputMsg;
    private TextView connectState;
    private BluetoothChatService mChatService = null;
    private String mConnectedDeviceName = null;
    // 传出消息的字符串缓冲区
    private StringBuffer mOutStringBuffer;
    private String userMsg = "";

    private DrawView drawView;
    private ConstraintLayout root;
    private Thread drawViewThread;

    private String readMessage;
    private String temperature_str = "";


    private Button autoMode;
    private Button mannualMode;
    private Button findlineMode;
    private SeekBar monitorUp;
    private SeekBar monitorLr;
    private Button forward;
    private Button back;
    private Button stop;
    private Button left;
    private Button right;

    private TextView temperature;
    private TextView humid;
    private TextView leftDis;
    private TextView rightDis;
    private TextView smokeCatogery;
    private TextView smokeConcentrate;
    private TextView fire;
    private TextView earthQuake;
    private TextView person;


    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                             WindowManager.LayoutParams.FLAG_FULLSCREEN ); /*隐藏状态栏 status bar*/
        setContentView(R.layout.main);
        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();  /*获取adapter*/
        scanButton = (Button)findViewById(R.id.scan_button);  /*扫描设备*/
        sendButton = (Button)findViewById(R.id.send_button);
        inputMsg = (EditText)findViewById(R.id.input_msg);
        connectState = (TextView)findViewById(R.id.connect_state);

        autoMode = (Button)findViewById(R.id.auto);
        mannualMode = (Button)findViewById(R.id.mannual);
        findlineMode = (Button)findViewById(R.id.find_line);
        monitorUp = (SeekBar)findViewById(R.id.monitor_ud);
        monitorLr = (SeekBar)findViewById(R.id.monitor_lr);
        forward = (Button)findViewById(R.id.forward);
        back = (Button)findViewById(R.id.back);
        stop = (Button)findViewById(R.id.stop);
        left = (Button)findViewById(R.id.turn_left);
        right = (Button)findViewById(R.id.turn_right);

        temperature = (TextView)findViewById(R.id.temperature);
        humid = (TextView)findViewById(R.id.humid);
        leftDis = (TextView)findViewById(R.id.left_dis);
        rightDis = (TextView)findViewById(R.id.right_dis);
        smokeCatogery = (TextView)findViewById(R.id.smoke_cat);
        smokeConcentrate = (TextView)findViewById(R.id.smoke_concentrate);
        fire = (TextView)findViewById(R.id.fire);
        earthQuake = (TextView)findViewById(R.id.earthquake);
        person = (TextView)findViewById(R.id.person);

        root = (ConstraintLayout)findViewById(R.id.root);
        recv = (EditText) findViewById(R.id.recv);

        recv.setGravity(Gravity.TOP);
        recv.setSelection(recv.getText().length(), recv.getText().length());
        recv.clearFocus();
        recv.setFocusable(false);


        /*一旦在主线程里面绘制界面，就会卡死主界面，故暂时放弃*/
        /* drawView = new DrawView(this);
        drawView.setOnTouchListener(new View.OnTouchListener()
        {
            @Override
            public boolean onTouch(View v, MotionEvent event)
            {
                if(event.getAction() == MotionEvent.ACTION_UP)
                {
                    float x = event.getX();
                    float y = event.getY();
                    if(drawView.isInCircle(x, y))
                    {
                        int flag = drawView.whichEdge(x, y);
                        Log.d("---------> flag: ", String.valueOf(flag));
                    }
                }
                return true;
            }
        });
        root.addView(drawView);*/

    }
    public void scanDevice()
    {
        Intent deviceListIntent = new Intent(this, DeviceList.class);
        startActivityForResult(deviceListIntent, REQUEST_CONNECT_DEVICE);
    }

    public View.OnClickListener onClickListener = new View.OnClickListener()
    {
        @Override
        public void onClick(View v)
        {
            switch (v.getId())
            {
                case R.id.scan_button: scanDevice(); break;
                case R.id.auto: autoMode(); break;
                case R.id.mannual: mannualMode(); break;
                case R.id.find_line: findlineMode(); break;
                case R.id.forward: goForward();break;
                case R.id.back: goBack();break;
                case R.id.stop: carStop();break;
                case R.id.turn_left: turnLeft();break;
                case R.id.turn_right: turnRight();break;
            }
        }
    };

    public SeekBar.OnSeekBarChangeListener onSeekBarChangeListener = new SeekBar.OnSeekBarChangeListener()
    {
        @Override
        public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
            switch (seekBar.getId())
            {
                case R.id.monitor_ud:  process_sg90_ud(progress); break;
                case R.id.monitor_lr: process_sg90_lr(progress); break;
            }
        }

        @Override
        public void onStartTrackingTouch(SeekBar seekBar) {
        }

        @Override
        public void onStopTrackingTouch(SeekBar seekBar) {
        }
    };

    public void goForward()
    {
        sendMessage("*aDaPP#");
        //Log.d("--->goForward", "*aDaPP#");
    }
    public void goBack()
    {
        sendMessage("*aEaPP#");
    }
    public void carStop()
    {
        sendMessage("*aCaPP#");
    }
    public void turnLeft()
    {
        sendMessage("*aHaPP#");
    }
    public void turnRight()
    {
        sendMessage("*aIaPP#");
    }
    public void autoMode()
    {
        sendMessage("*bBabc#");
    }
    public void mannualMode()
    {
        sendMessage("*aCabc#");
    }
    public void findlineMode()
    {
        sendMessage("*cCabc#");
    }
    public void process_sg90_ud(int progress)
    {
        char sg90_up_val = (char)(progress + 30);
        String sg90_head = "*aJa";
        String sg90_middle = String.valueOf(sg90_up_val);
        String sg90_tail = "a#";
        String sg90ud_all = sg90_head + sg90_middle + sg90_tail;
        sendMessage(sg90ud_all);
        //Log.d("-->sg90ud", sg90ud_all);
    }
    public void process_sg90_lr(int progress)
    {
        char sg90_lr_val = (char)(progress+50);
        String sg90lr_head = "*aKa";
        String sg90lr_middle = String.valueOf(sg90_lr_val);
        String sg90lr_tail = "a#";
        String sg90lr_all = sg90lr_head + sg90lr_middle + sg90lr_tail;
        sendMessage(sg90lr_all);
        //Log.d("-->sg90_ud", sg90lr_all);
    }

    /* 处理后台线程发过来的信息 */
    private final Handler mHandler = new Handler()
    {
        @Override
        public void handleMessage(Message msg)
        {
            switch (msg.what)
            {
                case MESSAGE_STATE_CHANGE:
                    switch (msg.arg1)
                    {
                        case BluetoothChatService.STATE_CONNECTED:
                            connectState.setText("连好了");break;
                        case BluetoothChatService.STATE_CONNECTING:
                            connectState.setText("正在拼命连接"); break;
                        case BluetoothChatService.STATE_LISTEN: break;
                        case BluetoothChatService.STATE_NONE:
                            connectState.setText("未连接"); break;
                    }   break;
                /*发送数据*/
                case MESSAGE_WRITE:
                    {
                        byte[] writeBuf = (byte[]) msg.obj;
                        // 构建一个字符串缓冲区
                        String writeMessage = new String(writeBuf);
                        recv.getText().append("\n<--" + writeMessage + "\n");
                        break;
                    }

                /*接收后台数据*/
                case MESSAGE_READ:
                    byte[] readBuf = (byte[]) msg.obj;
                    readMessage = new String(readBuf, 0, msg.arg1);
                    processRecvData(readMessage);
                    //Log.d("recv....", readMessage);
                    break;
                case MESSAGE_DEVICE_NAME:
                    mConnectedDeviceName = msg.getData().getString(DEVICE_NAME); // 保存该连接装置的名字
                    Toast.makeText(getApplicationContext(), "已连接 " + mConnectedDeviceName, Toast.LENGTH_SHORT).show();
                    break;
                case MESSAGE_TOAST:
                    Toast.makeText(getApplicationContext(), msg.getData().getString(TOAST), Toast.LENGTH_SHORT).show();
                    break;
            }
        }
    };

    public void processRecvData(String recvMsg)
    {
        /*--------------------recv protocol-----------------*/
        /*device*/
        final char Dht11 = 0x30;          // 字符 0
        final char MQ_2 = 0x31;           //1
        final char PIR = 0x32;            //2
        final char SHOCK = 0x33;          //3
        final char FIRE = 0x34;           //4
        final char L_ULTRA = 0x35;        //5
        final char R_ULTRA = 0x36;        //6

        /*data flag*/
        final char TEMP = 0x41;       //A
        final char HUMID = 0x42;      //B
        final char TTL = 0x43;        //C
        final char NORMAL = 0x44;     //D
        final char FLAG = 0x45;       //E

        if(recvMsg.length() == 12)
        {
            if((recvMsg.charAt(2) == '*') && (recvMsg.charAt(9) == '#'))
            {
                recv.append(recvMsg);

                switch (recvMsg.charAt(3))
                {
                    case Dht11:
                        switch (recvMsg.charAt(4))
                        {
                            case TEMP: showTemperature(recvMsg); break;
                            case HUMID: showHumid(recvMsg); break;
                        }   break;
                    case MQ_2:
                        switch (recvMsg.charAt(4))
                        {
                            case NORMAL: showSmokeConcentrate(recvMsg); break;
                            case FLAG: showSmokeCategory(recvMsg); break;
                        }   break;
                    case L_ULTRA: showLeftDis(recvMsg); break;
                    case R_ULTRA: showRightDis(recvMsg); break;
                }
            }
        }
        if(recvMsg.length() == 9)
        {
            if((recvMsg.charAt(2) == '*') && (recvMsg.charAt(6) == '#'))
            {
                recv.append(recvMsg);
                switch (recvMsg.charAt(3))
                {
                    case PIR: showPir(recvMsg); break;
                    case SHOCK: showShock(recvMsg); break;
                    case FIRE: showFire(recvMsg); break;
                }
            }
        }
    }

    public String get_long_data(String recvMsg)
    {
        char[] tempChar = {recvMsg.charAt(5), recvMsg.charAt(6), recvMsg.charAt(7), recvMsg.charAt(8)};
        return new String(tempChar);
    }
    public String get_short_data(String recvMsg)
    {
        char[] tempChar2 = {recvMsg.charAt(5)};
        return new String (tempChar2);
    }

    public void showTemperature(String recvMsg)
    {
        String tempStr = "";
        tempStr = get_long_data(recvMsg);
        temperature.setText(tempStr);
    }
    public void showHumid(String recvMsg)
    {
        String humidStr = "";
        humidStr = get_long_data(recvMsg);
        humid.setText(humidStr);
    }
    public void showLeftDis(String recvMsg)
    {
        String leftStr = get_long_data(recvMsg);
        leftDis.setText(leftStr);
    }
    public void showRightDis(String recvMsg)
    {
        String rightStr = get_long_data(recvMsg);
        rightDis.setText(rightStr);
    }
    public void showSmokeConcentrate(String recvMsg)
    {
        String somkeConStr = get_long_data(recvMsg);
        smokeConcentrate.setText(somkeConStr);
    }


    public void showSmokeCategory(String recvMsg)
    {
        String smokeCatStr = get_short_data(recvMsg);
        int parseCat = Integer.parseInt(smokeCatStr);
        switch (parseCat)
        {
            case 0: smokeCatogery.setText("无"); break;
            case 1: smokeCatogery.setText("甲烷"); break;
            case 2: smokeCatogery.setText("丁烷"); break;
            case 3: smokeCatogery.setText("酒精"); break;
        }
    }
    public void showPir(String recvMsg)
    {
        String pirStr = get_short_data(recvMsg);
        person.setText(pirStr);
    }
    public void showShock(String recvMsg)
    {
        String shockStr = get_short_data(recvMsg);
        earthQuake.setText(shockStr);
    }
    public void showFire(String recvMsg)
    {
        String fireStr = get_short_data(recvMsg);
        fire.setText(fireStr);
    }

    @Override
    protected void onStart()
    {
        super.onStart();

        scanButton.setOnClickListener(onClickListener);
        autoMode.setOnClickListener(onClickListener);
        mannualMode.setOnClickListener(onClickListener);
        findlineMode.setOnClickListener(onClickListener);
        monitorUp.setOnSeekBarChangeListener(onSeekBarChangeListener);
        monitorLr.setOnSeekBarChangeListener(onSeekBarChangeListener);
        forward.setOnClickListener(onClickListener);
        back.setOnClickListener(onClickListener);
        stop.setOnClickListener(onClickListener);
        left.setOnClickListener(onClickListener);
        right.setOnClickListener(onClickListener);

        if(!mBluetoothAdapter.isEnabled())
        {
            Intent openBluetoothIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(openBluetoothIntent, REQUEST_ENABLE_BT);
        }else{
            if(mChatService == null)
            {
                setupChat();
            }
        }
    }
    /*处理所有从其他界面返回的结果数据*/
    public void onActivityResult(int requestCode, int resultCode, Intent data)
    {
        switch (requestCode)
        {
            case REQUEST_CONNECT_DEVICE:
                if (resultCode == Activity.RESULT_OK)
                {
                    String address = data.getExtras().getString(DeviceList.EXTRA_DEVICE_ADDRESS);// 获得设备地址
                    BluetoothDevice device = mBluetoothAdapter.getRemoteDevice(address); // 把蓝牙设备对象
                    mChatService.connect(device);  // 试图连接到装置
                }   break;
            case REQUEST_ENABLE_BT:
                if (resultCode == Activity.RESULT_OK)  // 当请求启用蓝牙返回
                {
                    setupChat();
                }   break;
        }
    }
    private void setupChat()
    {
        sendButton.setOnClickListener(new View.OnClickListener()
        {
            public void onClick(View v)
            {
                sendMessage(inputMsg.getText().toString());
            }
        });
        // 初始化bluetoothchatservice执行蓝牙连接
        mChatService = new BluetoothChatService(this, mHandler);
        // 缓冲区初始化传出消息
        mOutStringBuffer = new StringBuffer("");

    }
    /*发送字符串*/
    public void sendMessage(String message) {
        // 检查我们实际上在任何连接
        if (mChatService.getState() != BluetoothChatService.STATE_CONNECTED) {
            Toast.makeText(this, R.string.not_connected, Toast.LENGTH_SHORT).show();
            return;
        }

        char[] tail_data = {0x0D, 0x0A};
        String tail = new String(tail_data);
        String all_message = message + tail;

        if ((all_message.length() - 2) > 0)
        {
            byte[] send = all_message.getBytes();
            mChatService.write(send);
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (mChatService != null)
        {
            if (mChatService.getState() == BluetoothChatService.STATE_NONE) {
                // 启动蓝牙聊天服务
                mChatService.start();
            }
        }
    }
}
