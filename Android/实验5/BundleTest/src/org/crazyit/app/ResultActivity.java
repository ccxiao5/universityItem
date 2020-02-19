/**
 *
 */
package org.crazyit.app;

import org.crazyt.model.Person;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

/**
 * Description:
 * <br/>��վ: <a href="http://www.crazyit.org">���Java����</a>
 * <br/>Copyright (C), 2001-2014, Yeeku.H.Lee
 * <br/>This program is protected by copyright laws.
 * <br/>Program Name:
 * <br/>Date:
 * @author  Yeeku.H.Lee kongyeeku@163.com
 * @version  1.0
 */
public class ResultActivity extends Activity
{
	EditText name1_new;
	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.result);
		TextView name = (TextView) findViewById(R.id.name);
		TextView passwd = (TextView) findViewById(R.id.passwd);
		TextView gender = (TextView) findViewById(R.id.gender);
		// ��ȡ������Result��Intent
		Intent intent = getIntent();
		// ֱ��ͨ��Intentȡ������Я����Bundle���ݰ��е�����
		Person p = (Person) intent.getSerializableExtra("person");
		name.setText("�����û���Ϊ��" + p.getName());
		passwd.setText("��������Ϊ��" + p.getPass());
		gender.setText("�����Ա�Ϊ��" + p.getGender());
		
		Button bn1=(Button) findViewById(R.id.bn1);	
		bn1.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				name1_new=(EditText) findViewById(R.id.name1_new);
				Intent intent = getIntent();
				intent.putExtra("name",
						name1_new.getText().toString());
				// ���ø�SelectActivity�Ľ���룬�����ý���֮���˻ص�Activity
				ResultActivity.this.setResult(0, intent);
				// ����SelectCityActivity��**************************************************************************
				ResultActivity.this.finish();	
			}
		});
	}
	
	
}
