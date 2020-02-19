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
 * <br/>网站: <a href="http://www.crazyit.org">疯狂Java联盟</a>
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
		// 获取启动该Result的Intent
		Intent intent = getIntent();
		// 直接通过Intent取出它所携带的Bundle数据包中的数据
		Person p = (Person) intent.getSerializableExtra("person");
		name.setText("您的用户名为：" + p.getName());
		passwd.setText("您的密码为：" + p.getPass());
		gender.setText("您的性别为：" + p.getGender());
		
		Button bn1=(Button) findViewById(R.id.bn1);	
		bn1.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				name1_new=(EditText) findViewById(R.id.name1_new);
				Intent intent = getIntent();
				intent.putExtra("name",
						name1_new.getText().toString());
				// 设置该SelectActivity的结果码，并设置结束之后退回的Activity
				ResultActivity.this.setResult(0, intent);
				// 结束SelectCityActivity。**************************************************************************
				ResultActivity.this.finish();	
			}
		});
	}
	
	
}
