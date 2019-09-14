package com.example.smarthome;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import com.loopj.android.http.AsyncHttpClient;
import com.loopj.android.http.JsonHttpResponseHandler;

import org.json.JSONObject;

import cz.msebera.android.httpclient.Header;

public class MainActivity extends AppCompatActivity {

    private String BASE_URL;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        BASE_URL = "http://" + "192.168.1.26" + ":" + String.valueOf(getResources().getInteger(R.integer.Port));

        Button on = (Button) findViewById(R.id.on);
        Button off = (Button) findViewById(R.id.off);

        on.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                requestData(BASE_URL + "/on");
            }
        });

        off.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                requestData(BASE_URL + "/off");
            }
        });

    }

    private void requestData(String url) {
        //Everything below is part of the Android Asynchronous HTTP Client

        AsyncHttpClient client = new AsyncHttpClient();

        client.get(url, new JsonHttpResponseHandler() {

            @Override
            public void onSuccess(int statusCode, Header[] headers, JSONObject response) {

                Log.d("PuRo", "JSON: " + response.toString());

                try {

                    Toast.makeText(MainActivity.this, "value: " + response.getString("value"), Toast.LENGTH_SHORT).show();

                } catch (Exception e) {

                    Log.e("PuRo", e.toString());

                }

            }

            @Override
            public void onFailure(int statusCode, Header[] headers, Throwable e, JSONObject response) {

                Toast.makeText(MainActivity.this, "Request Failed", Toast.LENGTH_SHORT).show();
            }

        });

    }

}
