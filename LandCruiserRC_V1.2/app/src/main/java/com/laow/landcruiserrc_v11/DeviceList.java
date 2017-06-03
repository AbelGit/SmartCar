package com.laow.landcruiserrc_v11;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.util.Set;

public class DeviceList extends AppCompatActivity {

    private static final String TAG = "DeviceList";
    private static final boolean D = true;

    public static String EXTRA_DEVICE_ADDRESS = "device_address";

    private BluetoothAdapter mBtAdapter;
    private ArrayAdapter<String> mPairedDevicesArrayAdapter;
    private ArrayAdapter<String> mNewDevicesArrayAdapter;

    private Button scanNewDevice;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        //requestWindowFeature(Window.FEATURE_ACTION_BAR);
        setContentView(R.layout.device_list);
        scanNewDevice = (Button)findViewById(R.id.scan_new_device);
        scanNewDevice.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v) {
                doDiscovery();
                v.setVisibility(View.GONE);
            }
        });
        /*初始化数组适配器。一个已配对装置和一个新发现的设备*/
        mPairedDevicesArrayAdapter = new ArrayAdapter<String>(this, R.layout.device_name);
        mNewDevicesArrayAdapter = new ArrayAdapter<String>(this, R.layout.device_name);
        /*寻找和建立配对设备列表*/
        ListView pairedListView = (ListView) findViewById(R.id.paired_devices);
        pairedListView.setAdapter(mPairedDevicesArrayAdapter);
        pairedListView.setOnItemClickListener(mDeviceClickListener);  //监听器对象, 连接指定的设备
        /*寻找和建立为新发现的设备列表*/
        ListView newDevicesListView = (ListView) findViewById(R.id.new_devices);
        newDevicesListView.setAdapter(mNewDevicesArrayAdapter);
        newDevicesListView.setOnItemClickListener(mDeviceClickListener); //监听器对象, 连接指定的设备
        /*手机系统广播, 进行注册, receiver响应设备*/
        IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND);
        this.registerReceiver(mReceiver, filter);
        /*广播时发现已完成注册*/
        filter = new IntentFilter(BluetoothAdapter.ACTION_DISCOVERY_FINISHED);
        this.registerReceiver(mReceiver, filter);
        /*获取本地蓝牙适配器*/
        mBtAdapter = BluetoothAdapter.getDefaultAdapter();
        /*得到一套目前配对设备*/
        Set<BluetoothDevice> pairedDevices = mBtAdapter.getBondedDevices();
        /*If there are paired devices, add each one to the ArrayAdapter*/
        if (pairedDevices.size() > 0) {
            findViewById(R.id.title_paired_devices).setVisibility(View.VISIBLE);
            for (BluetoothDevice device : pairedDevices) {
                mPairedDevicesArrayAdapter.add(device.getName() + "\n" + device.getAddress());
            }
        } else {
            String noDevices = "没有已配对设备";
            mPairedDevicesArrayAdapter.add(noDevices);
        }
    }
    private void doDiscovery()      /*扫描新设备*/
    {
        /*显示扫描的称号*/
        setProgressBarIndeterminateVisibility(true);
        setTitle(R.string.scanning);
        /*打开新设备的字幕*/
        findViewById(R.id.title_new_devices).setVisibility(View.VISIBLE);
        /*如果我们已经发现，阻止它*/
        if (mBtAdapter.isDiscovering()) {
            mBtAdapter.cancelDiscovery();
        }
        mBtAdapter.startDiscovery();
    }
    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (mBtAdapter != null)
        {
            mBtAdapter.cancelDiscovery();
        }
        this.unregisterReceiver(mReceiver); /*杀掉receiver*/
    }

    /*设备点击监听器*/
    private AdapterView.OnItemClickListener mDeviceClickListener = new AdapterView.OnItemClickListener()
    {
        public void onItemClick(AdapterView<?> av, View v, int arg2, long arg3) {
            /*新设备列表出现后, 点击相应的条目后就不需要再发现其他的设备了, 对话框消失*/
            mBtAdapter.cancelDiscovery();
            /*获得设备MAC地址，这是近17字的视图*/
            String info = ((TextView) v).getText().toString();
            String address = info.substring(info.length() - 17);
            /*创建结果意图和包括地址*/
            Intent intent = new Intent();
            intent.putExtra(EXTRA_DEVICE_ADDRESS, address);
            /*结果，完成这项活动*/
            setResult(Activity.RESULT_OK, intent);
            finish();
        }
    };

    /*receiver接收设备发来的名字, 地址, 并加入newAdapter */
    private final BroadcastReceiver mReceiver = new BroadcastReceiver()
    {
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if (BluetoothDevice.ACTION_FOUND.equals(action))
            {
                BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                if (device.getBondState() != BluetoothDevice.BOND_BONDED) {
                    mNewDevicesArrayAdapter.add(device.getName() + "\n" + device.getAddress());
                }
            } else if (BluetoothAdapter.ACTION_DISCOVERY_FINISHED.equals(action)) {
                setProgressBarIndeterminateVisibility(false);
                setTitle(R.string.select_device);
                if (mNewDevicesArrayAdapter.getCount() == 0) {
                    String noDevices = "啥都没发现";
                    mNewDevicesArrayAdapter.add(noDevices);
                }
            }
        }
    };

}
