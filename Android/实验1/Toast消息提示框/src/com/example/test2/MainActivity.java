package com.example.test2;


import android.app.Activity;
import android.graphics.Color;
import android.os.Bundle;
import android.view.Gravity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;


public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        Button bt1=(Button) findViewById(R.id.button1);
        Button bt2=(Button) findViewById(R.id.button2);
        
        bt1.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				
				Toast toast =Toast.makeText(MainActivity.this,"程虎威_151001215",Toast.LENGTH_LONG);			
				toast.show();
			}
		});
        bt2.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				// 创建一个Toast提示信息
				Toast toast = new Toast(MainActivity.this);
				// 设置Toast的显示位置
				toast.setGravity(Gravity.CENTER, 0, 0);
				// 创建一个ImageView
				ImageView image = new ImageView(MainActivity.this);
				image.setImageResource(R.drawable.ic_launcher);
				// 创建一个LinearLayout容器
				LinearLayout ll = new LinearLayout(MainActivity.this);
				// 向LinearLayout中添加图片、原有的View
				ll.addView(image);
				// 创建一个ImageView
				TextView textView = new TextView(MainActivity.this);
				textView.setText("程虎威_151001215");
				// 设置文本框内字体的大小和颜色
				textView.setTextSize(30);
				textView.setTextColor(Color.GREEN);
				textView.setBackgroundColor(Color.WHITE);
				ll.addView(textView);
				// 设置Toast显示自定义View				
				toast.setView(ll);
				// 设置Toast的显示时间
				toast.setDuration(Toast.LENGTH_LONG);
				toast.show();	
			}
		});
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();
        if (id == R.id.action_settings) {
            return true;
        }
        return super.onOptionsItemSelected(item);
    }
}
