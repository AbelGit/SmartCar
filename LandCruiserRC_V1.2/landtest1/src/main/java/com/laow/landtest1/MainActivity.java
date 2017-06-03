package com.laow.landtest1;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import static android.R.attr.button;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        Button button = (Button)findViewById(R.id.button);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                char [] tail = {0x0D, 0x0A};
                String tail_str = new String(tail);
                String world = "world";
                String add = world + tail_str;
                String length = String.valueOf(add.length());

                Log.i("---->hello: ", length);
            }
        });
    }

}
