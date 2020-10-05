package com.imdp.salte;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import com.github.mikephil.charting.charts.LineChart;
import com.google.android.material.bottomnavigation.BottomNavigationView;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallbackExtended;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.json.JSONObject;

public class MainActivity extends AppCompatActivity {
    MqttHelper mqttHelper;
    ChartHelper mChart;
    LineChart chart;
    String nacl11,nacl12,nacl13,baume ;
    TextView nacl2;
    TextView nacl1;
    TextView nacl3;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        nacl1 = (TextView) findViewById(R.id.nacl1);
        nacl2 = (TextView) findViewById(R.id.nacl2);
        nacl3 = (TextView) findViewById(R.id.nacl3);
        chart = (LineChart) findViewById(R.id.salch);
        mChart = new ChartHelper(chart);
        startMqtt();
    }

    private void startMqtt() {
        mqttHelper = new MqttHelper(getApplicationContext());
        mqttHelper.mqttAndroidClient.setCallback(new MqttCallbackExtended() {
            @Override
            public void connectComplete(boolean b, String s) {
                Log.w("Debug", "Connected");
            }

            @Override
            public void connectionLost(Throwable throwable) {

            }

            @Override
            public void messageArrived(String topic, MqttMessage mqttMessage) throws Exception {
                Log.w("Debug", mqttMessage.toString());
                switch (topic.toString()){
                    case "sim7000e/generic_device/v4/common":
                        JSONObject obj = new JSONObject(mqttMessage.toString());
                        nacl13 = obj.getString("nacl");
                        nacl12 = obj.getString("nacl");
                        nacl11 = obj.getString("nacl");
                        baume = obj.getString("baume");
                         mChart.addEntry(Float.valueOf(baume));
                        nacl1.setText(nacl11);    //aslinya ("Flow Sensor: "+flow)
                        nacl2.setText(nacl12);
                        nacl3.setText(nacl13);
                        break;
                    default:
                        Log.d("Error","Error ocquired");
                }
            }

            @Override
            public void deliveryComplete(IMqttDeliveryToken iMqttDeliveryToken) {
            }
        });
    }
}