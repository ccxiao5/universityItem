package org.crazyit.app;

import org.crazyt.model.Person;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RadioButton;
import android.widget.Toast;

/**
 * Description:
 * <br/>site: <a href="http://www.crazyit.org">crazyit.org</a>
 * <br/>Copyright (C), 2001-2014, Yeeku.H.Lee
 * <br/>This program is protected by copyright laws.
 * <br/>Program Name:
 * <br/>Date:
 * @author  Yeeku.H.Lee kongyeeku@163.com
 * @version  1.0
 */
public class BundleTest extends Activity
{
	EditText name;
	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		Button bn = (Button) findViewById(R.id.bn);
		bn.setOnClickListener(new OnClickListener()
		{
			public void onClick(View v)
			{
				name = (EditText)findViewById(R.id.name);
				EditText passwd = (EditText)findViewById(R.id.passwd);
				RadioButton male = (RadioButton) findViewById(R.id.male);
				String gender = male.isChecked() ? "男 " : "女";
				Person p = new Person(name.getText().toString(), passwd
						.getText().toString(), gender);
				// 创建一个Bundle对象
				Bundle data = new Bundle();
				data.putSerializable("person", p);
				// 创建一个Intent
				Intent intent = new Intent(BundleTest.this,
						ResultActivity.class);
				intent.putExtras(data);
				// 启动intent对应的Activity
				startActivityForResult(intent, 0);
			}
		});
		
	}
	@Override
	public void onActivityResult(int requestCode
		, int resultCode, Intent intent)
	{
		// 当requestCode、resultCode同时为0，也就是处理特定的结果
		if (requestCode==0&&resultCode == 0)
		{
			// 取出Intent里的Extras数据
			Bundle data = intent.getExtras();
			// 取出Bundle中的数据
			String resultCity = data.getString("name");	
			name.setText(resultCity);
		}
		
	}
}