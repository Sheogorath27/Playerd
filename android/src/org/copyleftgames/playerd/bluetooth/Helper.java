package org.copyleftgames.playerd.bluetooth;

import android.annotation.TargetApi;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.ParcelUuid;

import java.io.IOException;
import java.util.ArrayList;
import java.util.UUID;

public class Helper extends Activity {
   BluetoothAdapter blueadapter = BluetoothAdapter.getDefaultAdapter();
   ArrayList<BluetoothDevice> devlist = new ArrayList<BluetoothDevice>();
    IntentFilter discoverFilter;
    IntentFilter sdpFilter;
    IntentFilter pairFilter;
    DeviceReceiver discoverReceiver;
    DeviceReceiver sdpReceiver;
    DeviceReceiver pairReceiver;
   Boolean isDiscovering = false;
   Boolean isSDPsearching = false;
   Boolean isPairing = false;
    ParcelUuid[] servicerecord;
    BluetoothSocket sock;

   boolean check_adapter()
   {
      if (blueadapter == null)
         return false;
      return true;
   }

   void bt_enable()
   {
      if (!blueadapter.isEnabled())
      {
         Intent enablebtintent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
         startActivity(enablebtintent);
      }
   }

    void bt_discoverable()
    {
        Intent discoverableintent = new Intent(BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);
        startActivity(discoverableintent);
    }

    void start_discovery()
    {
        if (!check_adapter())
            return;
        bt_enable();
        discoverFilter = new IntentFilter();
        discoverReceiver = new DeviceReceiver();
        discoverFilter.addAction(BluetoothAdapter.ACTION_DISCOVERY_FINISHED);
        discoverFilter.addAction(BluetoothDevice.ACTION_FOUND);
        registerReceiver(discoverReceiver, discoverFilter);
        if (!blueadapter.startDiscovery())
        {
            //error in starting
        }
        isDiscovering = true;
    }

    @TargetApi(19)
    void start_pairing(String btaddressstr)
    {
        BluetoothDevice dev = blueadapter.getRemoteDevice(btaddressstr);
        if (dev.getBondState() != BluetoothDevice.BOND_NONE)
            return; //already bonded or bonding
        pairFilter = new IntentFilter();
        pairReceiver = new DeviceReceiver();
        pairFilter.addAction(BluetoothDevice.ACTION_BOND_STATE_CHANGED);
        registerReceiver(pairReceiver, pairFilter);
        if (!dev.createBond())
        {
            //return error
        }
        isPairing = true;
    }

//could not find removebond method
//    void remove_pair(string btaddressstr)
//    {
//        BluetoothDevice dev = blueadapter.getRemoteDevice(btaddressstr);
//        if (dev.getBondState() == BluetoothDevice.BOND_NONE)
//        {
//            return;
//        }
//        else if (dev.getBondState() == BluetoothDevice.BOND_BONDING)
//        {
//
//        }
//    }

    @TargetApi(15)
    void start_sdp(String btaddressstr)
    {
        if (!check_adapter())
            return;
        bt_enable();
        BluetoothDevice dev = blueadapter.getRemoteDevice(btaddressstr);
        if (dev.getUuids() != null)
        {
            servicerecord = dev.getUuids();
            return;
        }
        sdpFilter = new IntentFilter();
        sdpReceiver = new DeviceReceiver();
        sdpFilter.addAction(BluetoothDevice.ACTION_UUID);
        registerReceiver(sdpReceiver, sdpFilter);
        if (!dev.fetchUuidsWithSdp())
        {
            //return error
        }
        isSDPsearching = true;
    }

    //auto pair because it is secure
    void connect_rfcomm(String btaddressstr, UUID service)
    {
        if (!check_adapter())
        {
            return;
        }
        bt_enable();
        BluetoothDevice dev = blueadapter.getRemoteDevice(btaddressstr);

        try
        {
           sock =  dev.createRfcommSocketToServiceRecord(service);
        }
        catch (IOException e)
        {
            try
            {
                sock.close();
            }
            catch (IOException eclose){}
            return;
        }

        if (isDiscovering)
            blueadapter.cancelDiscovery();

        try
        {
            sock.connect();
        }
        catch (IOException econ)
        {
            try
            {
                sock.close();
            }
            catch (IOException eclose){}
        }
        //do something with connection
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (isDiscovering)
        {
            unregisterReceiver(discoverReceiver);
            //
        }

        if (isSDPsearching)
        {
            unregisterReceiver(sdpReceiver);
            //
        }

        if (isPairing)
        {
            unregisterReceiver(pairReceiver);
        }
    }

    class DeviceReceiver extends BroadcastReceiver
    {
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if (BluetoothDevice.ACTION_FOUND.equals(action))
            {
                BluetoothDevice dev = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                devlist.add(dev);
            }

            else if (BluetoothAdapter.ACTION_DISCOVERY_FINISHED.equals(action))
            {
                isDiscovering = false;
                dev_parser();
            }

            else if (BluetoothDevice.ACTION_UUID.equals(action))
            {
                servicerecord = (ParcelUuid[]) intent.getParcelableArrayExtra(BluetoothDevice.EXTRA_UUID);
                isSDPsearching = false;
                sdp_parser();

            }

            else if (BluetoothDevice.ACTION_BOND_STATE_CHANGED.equals(action))
            {
                //pairing complete
                isPairing = false;
                if (intent.getIntExtra(BluetoothDevice.EXTRA_BOND_STATE, 0) != BluetoothDevice.BOND_BONDED)
                {
                    //error not paired
                }
            }

        }
    }

    void dev_parser()
    {
        //will add depending on JNI implementation
    }

    void sdp_parser()
    {
        //will add depending on JNI implementation
    }
}
