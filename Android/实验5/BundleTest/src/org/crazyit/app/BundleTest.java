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
				String gender = male.isChecked() ? "�� " : "Ů";
				Person p = new Person(name.getText().toString(), passwd
						.getText().toString(), gender);
				// ����һ��Bundle����
				Bundle data = new Bundle();
				data.putSerializable("person", p);
				// ����һ��Intent
				Intent intent = new Intent(BundleTest.this,
						ResultActivity.class);
				intent.putExtras(data);
				// ����intent��Ӧ��Activity
				startActivityForResult(intent, 0);
			}
		});
		
	}
	@Override
	public void onActivityResult(int requestCode
		, int resultCode, Intent intent)
	{
		// ��requestCode��resultCodeͬʱΪ0��Ҳ���Ǵ����ض��Ľ��
		if (requestCode==0&&resultCode == 0)
		{
			// ȡ��Intent���Extras����
			Bundle data = intent.getExtras();
			// ȡ��Bundle�е�����
			String resultCity = data.getString("name");	
			name.setText(resultCity);
		}
		
	}
}