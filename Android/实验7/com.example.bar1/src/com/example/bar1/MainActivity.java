package com.example.bar1;
import java.util.ArrayList;  
import java.util.List;  
import java.util.Random;  
  
import org.achartengine.ChartFactory;  
import org.achartengine.GraphicalView;  
import org.achartengine.chart.ClickableArea;  
import org.achartengine.chart.LineChart;  
import org.achartengine.chart.PointStyle;  
import org.achartengine.chart.RangeBarChart;  
import org.achartengine.chart.BarChart.Type;  
import org.achartengine.chart.XYChart;  
import org.achartengine.model.RangeCategorySeries;  
import org.achartengine.model.XYMultipleSeriesDataset;  
import org.achartengine.model.XYSeries;  
import org.achartengine.renderer.SimpleSeriesRenderer;  
import org.achartengine.renderer.XYMultipleSeriesRenderer;  
import org.achartengine.renderer.XYSeriesRenderer;  

import com.example.bar1.R;
  
import android.app.ActionBar.LayoutParams;  
import android.app.Activity;  
import android.graphics.Canvas;  
import android.graphics.Color;  
import android.graphics.Paint;  
import android.graphics.Paint.Align;  
import android.os.Bundle;  
import android.util.Log;  
import android.view.Menu;  
import android.view.MenuItem;  
import android.view.View;  
import android.view.View.OnClickListener;  
import android.widget.Button;  
import android.widget.LinearLayout;  
  
public class MainActivity extends Activity {  
  
    private XYMultipleSeriesRenderer renderer;//2,XYMutilpleSeriesRenderer ���ڽ��л��Ƶ�����,��ӵ�XYSeriesRender����,���ڶ�����Ƶĵ㼯������  
    private XYMultipleSeriesDataset dataset;//�������ݵĴ��,��ӵ�XYseries����,�����ṩ���Ƶĵ㼯�ϵ�����  
    LinearLayout chart;  
      
      
//  @Override  
    protected void onCreate(Bundle savedInstanceState) {  
        super.onCreate(savedInstanceState);  
        setContentView(R.layout.activity_main);  
        chart=(LinearLayout) findViewById(R.id.chart);  
        Huatu(chart);  
          
    }  
          
          
        public void Huatu (LinearLayout chart){  
          
            dataset = new XYMultipleSeriesDataset();  
            renderer = new XYMultipleSeriesRenderer();  
          
            //��Χͼ����  
            double[] minValues = new double[] { -24, -19, -10, -1, 7, 12, 15, 14, 9, 1, -11, -16 };  
            double[] maxValues = new double[] { 7, 12, 24, 28, 33, 35, 37, 36, 28, 19, 11, 4 };  
  
            RangeCategorySeries series = new RangeCategorySeries("Temperature");  
            int length = minValues.length;  
            for (int k = 0; k < length; k++) {  
              series.add(minValues[k], maxValues[k]);  
            }  
            XYSeriesRenderer r = new XYSeriesRenderer();// ��Χͼ��������  
            r.setDisplayChartValues(true);  
            r.setChartValuesTextSize(30);  
            r.setChartValuesSpacing(3);  
            r.setGradientEnabled(true);  
            //������Ϊͼ����ɫ�Ľ���Ч��  
            r.setGradientStart(-20, Color.BLUE);  
            r.setGradientStop(20, Color.GREEN);  
              
              
            //��������  
            double[] Linedata={ 9, 10, 11, 15, 19, 23, 26, 25, 22, 18, 13, 10 };  
            XYSeries lineSeries=new XYSeries("Line");  
            for (int i = 1; i <=Linedata.length; i++) {  
                lineSeries.add(i, Linedata[i-1]);//������ӵ�iΪ�����ϵ�λ���±�  ��1��ʼ  ������2�������  
            }  
            XYSeriesRenderer lineRenderer=new XYSeriesRenderer();  
            lineRenderer.setColor(Color.BLUE);    
             
            lineRenderer.setPointStyle(PointStyle.SQUARE);   //������ʽ    
            lineRenderer = new XYSeriesRenderer();    
            lineRenderer.setColor(Color.RED);    
            lineRenderer.setPointStyle(PointStyle.CIRCLE);   
            lineRenderer.setDisplayChartValues(true);  
            lineRenderer.setChartValuesTextSize(30);  
             
              
            //��ͼ��������  
            renderer.setXLabelsPadding(20);//����X���±�ǩ��X��ľ���  
            renderer.setBarSpacing(0.2);//ͼ�ε���ʾ��С  ��ֵԽС ���Խ��  
            renderer.setXLabels(15);//X����ʾ���������  
            renderer.setYLabels(10);  
            renderer.setXAxisMin(0);//����X��������ʾ�ĵ�һ��������ֵ  ��0����0��ʼ����  
            renderer.setXAxisMax(12);  
            renderer.setYAxisMin(-5);  
//          renderer.setYAxisMax(600);  
              
            renderer.setPanEnabled(true, false);//ֻ������X�Ử��  
            //����X�������Ե�ı�ǩ  
//          renderer.addXTextLabel(1, "Jan");  
//          renderer.addXTextLabel(3, "Mar");  
//          renderer.addXTextLabel(5, "May");  
//          renderer.addXTextLabel(7, "Jul");  
//          renderer.addXTextLabel(10, "Oct");  
//          renderer.addXTextLabel(12, "Dec");  
//          renderer.addYTextLabel(-25, "Very cold");  
//          renderer.addYTextLabel(-10, "Cold");  
//          renderer.addYTextLabel(5, "OK");  
//          renderer.addYTextLabel(20, "Nice");  
            renderer.setMargins(new int[] {80, 60, 30, 0});//��  ��  �� ��   ͼ�����ܵķ�Χ  
//          renderer.setPanLimits(new double[] { 0, -20, 0, 720 });//���������ķ�Χ  
            renderer.setPanLimits(new double[] { -1, 24, 0, 500 });  
            renderer.setYLabelsAlign(Align.RIGHT);  
            
              
            //������ͼ����л���  
            dataset.addSeries(series.toXYSeries());  
            dataset.addSeries(lineSeries);  
            renderer.addSeriesRenderer(r);  
            renderer.addSeriesRenderer(lineRenderer);//��Ҫ���Ƶĵ���ӵ����������    
              
              
            String[] types={RangeBarChart.TYPE,LineChart.TYPE};  
            GraphicalView view=ChartFactory.getCombinedXYChartView(this, dataset, renderer, types);  
            chart.addView(view, LayoutParams.MATCH_PARENT,LayoutParams.MATCH_PARENT);
          }  
          
          
          
          
    }  