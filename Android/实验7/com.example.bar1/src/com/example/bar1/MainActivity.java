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
  
    private XYMultipleSeriesRenderer renderer;//2,XYMutilpleSeriesRenderer 用于进行绘制的设置,添加的XYSeriesRender对象,用于定义绘制的点集合设置  
    private XYMultipleSeriesDataset dataset;//用于数据的存放,添加的XYseries对象,用于提供绘制的点集合的数据  
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
          
            //范围图数据  
            double[] minValues = new double[] { -24, -19, -10, -1, 7, 12, 15, 14, 9, 1, -11, -16 };  
            double[] maxValues = new double[] { 7, 12, 24, 28, 33, 35, 37, 36, 28, 19, 11, 4 };  
  
            RangeCategorySeries series = new RangeCategorySeries("Temperature");  
            int length = minValues.length;  
            for (int k = 0; k < length; k++) {  
              series.add(minValues[k], maxValues[k]);  
            }  
            XYSeriesRenderer r = new XYSeriesRenderer();// 范围图参数设置  
            r.setDisplayChartValues(true);  
            r.setChartValuesTextSize(30);  
            r.setChartValuesSpacing(3);  
            r.setGradientEnabled(true);  
            //可以作为图表颜色的渐变效果  
            r.setGradientStart(-20, Color.BLUE);  
            r.setGradientStop(20, Color.GREEN);  
              
              
            //折线数据  
            double[] Linedata={ 9, 10, 11, 15, 19, 23, 26, 25, 22, 18, 13, 10 };  
            XYSeries lineSeries=new XYSeries("Line");  
            for (int i = 1; i <=Linedata.length; i++) {  
                lineSeries.add(i, Linedata[i-1]);//这里添加的i为坐标上的位置下标  从1开始  则代表第2个坐标点  
            }  
            XYSeriesRenderer lineRenderer=new XYSeriesRenderer();  
            lineRenderer.setColor(Color.BLUE);    
             
            lineRenderer.setPointStyle(PointStyle.SQUARE);   //设置样式    
            lineRenderer = new XYSeriesRenderer();    
            lineRenderer.setColor(Color.RED);    
            lineRenderer.setPointStyle(PointStyle.CIRCLE);   
            lineRenderer.setDisplayChartValues(true);  
            lineRenderer.setChartValuesTextSize(30);  
             
              
            //绘图参数设置  
            renderer.setXLabelsPadding(20);//设置X轴下标签与X轴的距离  
            renderer.setBarSpacing(0.2);//图形的显示大小  数值越小 面积越大  
            renderer.setXLabels(15);//X轴显示的坐标个数  
            renderer.setYLabels(10);  
            renderer.setXAxisMin(0);//设置X轴坐标显示的第一个坐标数值  从0即从0开始设置  
            renderer.setXAxisMax(12);  
            renderer.setYAxisMin(-5);  
//          renderer.setYAxisMax(600);  
              
            renderer.setPanEnabled(true, false);//只允许在X轴滑动  
            //设置X轴上特性点的标签  
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
            renderer.setMargins(new int[] {80, 60, 30, 0});//上  左  下 右   图表四周的范围  
//          renderer.setPanLimits(new double[] { 0, -20, 0, 720 });//设置拉动的范围  
            renderer.setPanLimits(new double[] { -1, 24, 0, 500 });  
            renderer.setYLabelsAlign(Align.RIGHT);  
            
              
            //将两个图表进行绘制  
            dataset.addSeries(series.toXYSeries());  
            dataset.addSeries(lineSeries);  
            renderer.addSeriesRenderer(r);  
            renderer.addSeriesRenderer(lineRenderer);//将要绘制的点添加到坐标绘制中    
              
              
            String[] types={RangeBarChart.TYPE,LineChart.TYPE};  
            GraphicalView view=ChartFactory.getCombinedXYChartView(this, dataset, renderer, types);  
            chart.addView(view, LayoutParams.MATCH_PARENT,LayoutParams.MATCH_PARENT);
          }  
          
          
          
          
    }  