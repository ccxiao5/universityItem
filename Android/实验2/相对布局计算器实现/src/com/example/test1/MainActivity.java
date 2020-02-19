package com.example.test1;

import android.app.Activity;
import android.os.Bundle;
import android.text.Editable;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;


public class MainActivity extends Activity {

    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Button bt_qingchu=(Button) findViewById(R.id.bt1);
        Button bt_chu=(Button) findViewById(R.id.bt2);
        Button bt_cheng=(Button) findViewById(R.id.bt3);
        Button bt_jian=(Button) findViewById(R.id.bt4);
        Button bt_jia=(Button) findViewById(R.id.bt5);
       
        bt_qingchu.setOnClickListener(new OnClickListener() {
       	 EditText et1=(EditText) findViewById(R.id.et1);
         EditText et2=(EditText) findViewById(R.id.et2);
         EditText et3=(EditText) findViewById(R.id.et3);
			@Override
			
			public void onClick(View v) {
				// TODO Auto-generated method stub
				et1.setText("");
				et2.setText("");
				et3.setText("");
			}
		});
        bt_chu.setOnClickListener(new OnClickListener() {
       	 EditText et1=(EditText) findViewById(R.id.et1);
         EditText et2=(EditText) findViewById(R.id.et2);
         EditText et3=(EditText) findViewById(R.id.et3);
			@Override
			public void onClick(View v) {
				String str1,str2,c;
				// TODO Auto-generated method stub
				str1=et1.getText().toString();
				str2=et2.getText().toString();
				double a,b;
				a=Double.valueOf(str1);
				b=Double.valueOf(str2);
				c=String.valueOf((a/b));
				et3.setText(c.toString());
			}
		});
        bt_cheng.setOnClickListener(new OnClickListener() {
       	 EditText et1=(EditText) findViewById(R.id.et1);
         EditText et2=(EditText) findViewById(R.id.et2);
         EditText et3=(EditText) findViewById(R.id.et3);
			@Override
			public void onClick(View v) {
				String str1,str2,c;
				// TODO Auto-generated method stub
				str1=et1.getText().toString();
				str2=et2.getText().toString();
				double a,b;
				a=Double.valueOf(str1);
				b=Double.valueOf(str2);
				c=String.valueOf((a*b));
				et3.setText(c.toString());
				}
		});
	 bt_jian.setOnClickListener(new OnClickListener() {
		 EditText et1=(EditText) findViewById(R.id.et1);
	     EditText et2=(EditText) findViewById(R.id.et2);
	     EditText et3=(EditText) findViewById(R.id.et3);
			@Override
			public void onClick(View v) {
				String str1,str2,c;
				// TODO Auto-generated method stub
				str1=et1.getText().toString();
				str2=et2.getText().toString();
				double a,b;
				a=Double.valueOf(str1);
				b=Double.valueOf(str2);
				c=String.valueOf((a-b));
				et3.setText(c.toString());
			}
		});
	 bt_jia.setOnClickListener(new OnClickListener() {
		 EditText et1=(EditText) findViewById(R.id.et1);
	     EditText et2=(EditText) findViewById(R.id.et2);
	     EditText et3=(EditText) findViewById(R.id.et3);
			@Override
			public void onClick(View v) {
				String str1,str2,c;
				// TODO Auto-generated method stub
				str1=et1.getText().toString();
				str2=et2.getText().toString();
				double a,b;
				a=Double.valueOf(str1);
				b=Double.valueOf(str2);
				c=String.valueOf((a+b));
				et3.setText(c.toString());
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
