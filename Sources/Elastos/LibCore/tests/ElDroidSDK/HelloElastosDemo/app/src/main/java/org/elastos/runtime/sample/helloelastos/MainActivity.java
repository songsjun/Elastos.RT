package org.elastos.runtime.sample.helloelastos;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;

import org.elastos.elastosrtsdk.HelloCarDemo;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        //Show the string that comes from a Elastos car objects[CHelloDemo]
        TextView textLabel = (TextView)findViewById(R.id.label);
        HelloCarDemo helloCarObj = new HelloCarDemo();
        String helloString = helloCarObj.hello();
        textLabel.setText(helloString);
    }
}
