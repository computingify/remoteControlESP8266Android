package com.computingify.paddleboatcontroler;

import android.content.Intent;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        TextView activationText = findViewById(R.id.wifiActivation);

        WifiManager wifiManager = (WifiManager) getApplicationContext().getSystemService(WIFI_SERVICE);
        if (wifiManager != null) {
            activationText.setText("Wifi activated");
            (findViewById(R.id.wifiActivationSuccess)).setVisibility(View.VISIBLE);

            Button connectionButton = findViewById(R.id.connectionButton);
            connectionButton.setVisibility(View.VISIBLE);
            connectionButton.setOnClickListener(v -> {
                TextView connectionText = findViewById(R.id.paddleConnection);
                WifiInfo info = wifiManager.getConnectionInfo();
                if (info != null) {
                    Communication comm = new Communication("192.168.4.1", 80);
                    new Thread(comm).start();

                    Handler handler = new Handler();
                    handler.postDelayed(() -> {
                        Intent intent = new Intent(this, RemoteControl.class);
                        startActivity(intent);
                    }, 1500);   //1,5 seconds

                } else {
                    connectionText.setText("Connection to the paddle Fail,\nNo wifi information");
                    (findViewById(R.id.wifiConnectionFailure)).setVisibility(View.VISIBLE);
                }
            });

        } else {
            activationText.setText("Wifi not activate, then try again");
            (findViewById(R.id.wifiActivationFailure)).setVisibility(View.VISIBLE);
        }
    }
}
