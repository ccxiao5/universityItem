package com.example.zuoye4;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.CheckBox;
import android.widget.RadioGroup;
import android.widget.RadioGroup.OnCheckedChangeListener;
import android.widget.TextView;


public class MainActivity extends Activity {

	RadioGroup rg;
	 TextView show;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
       
        rg=(RadioGroup) findViewById(R.id.rg);
        show=(TextView) findViewById(R.id.show);
        rg.setOnCheckedChangeListener(new OnCheckedChangeListener() {
			
			@Override
			public void onCheckedChanged(RadioGroup group, int checkedId) {
				// TODO Auto-generated method stub
				String trip=checkedId==R.id.male?
						"您的性别是男人": "您的性别是女人";
				CheckBox red1=(CheckBox) findViewById(R.id.red);
				if (red1.isChecked())
				{
					trip=trip+"+红色";
				}
				CheckBox blue1=(CheckBox) findViewById(R.id.blue);
				if (blue1.isChecked())
				{
					trip=trip+"+蓝色";
				}CheckBox green1=(CheckBox) findViewById(R.id.green);
				if (green1.isChecked())
				{
					trip=trip+"+绿色";
				}
				show.setText(trip);
						
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
