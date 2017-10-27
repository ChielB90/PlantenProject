package com.example.chileam.sesame;

import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.Toolbar;

import java.io.UnsupportedEncodingException;

import javax.crypto.SecretKey;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;

public class MainActivity extends AppCompatActivity {

    private static final String keyString = "8DF88E896F4CA4367F50D0FE75048026";
    private static final String ivString = "DB294FC2DAF41F8A";
    private static final String command = "OpenSesame";
    private EspClient client;
    private WifiManager manager;
    private Button b1, b2;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Toolbar appbar = (Toolbar) findViewById(R.id.app_bar);
        setActionBar(appbar);

        manager = (WifiManager) getSystemService(WIFI_SERVICE);
        WifiInfo info = manager.getConnectionInfo();
        b1 = (Button) findViewById(R.id.connect_button);
        b2 = (Button) findViewById(R.id.open_button);
        if (info.getSSID().indexOf(getString(R.string.SSID)) > 0) {
            b1.setVisibility(View.INVISIBLE);
            b2.setVisibility(View.VISIBLE);
        }
        new connectTask().execute("");
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case R.id.exit_now:
                HardExit();
                return true;
            default:
                return super.onOptionsItemSelected(item);
        }
    }

    public void Connect(View view) {
        WifiConfiguration conf = new WifiConfiguration();
        conf.SSID = String.format("\"%s\"", R.string.SSID);
        WifiInfo info = manager.getConnectionInfo();
        manager.setWifiEnabled(true);
        while (true) {
            Log.e("WIFI STATE", String.format("%s",manager.getWifiState()));
            if (manager.getWifiState() == WifiManager.WIFI_STATE_ENABLED) {
                if (info.getSSID().indexOf(getString(R.string.SSID)) > 0) {
                    break;
                }
            }
        }
        b1.setVisibility(View.INVISIBLE);
        b2.setVisibility(View.VISIBLE);
    }

    public void OpenSesame(View view) {
        try {
            byte[] bytes = keyString.getBytes("UTF-8");
            SecretKey key = new SecretKeySpec(bytes, "AES");
            bytes = ivString.getBytes("UTF-8");
            IvParameterSpec iv = new IvParameterSpec(bytes);
            SesameCrypto crypto = new SesameCrypto(key, iv);
            crypto.hashMSG(command);
            crypto.encrypHASH();
            crypto.packMSG(command);

            byte[] pack = crypto.encryptPack();
            if (client !=null) {
                b2.setEnabled(false);
                client.send(pack);
                client.close();
                b2.setEnabled(true);
                MainActivity.this.recreate();

            }
        }
        catch (UnsupportedEncodingException e) {
            Log.e("ERROR", e.toString());
            System.out.println(e.toString());
        }
    }

    public class connectTask extends AsyncTask<String,String,EspClient> {

        @Override
        protected EspClient doInBackground(String...msg) {
            client = new EspClient();
            Log.e("CLIENT", "Connect with: " + client.getIP());
            return null;
        }
    }

    private void HardExit() {
        finish();
    }
}
