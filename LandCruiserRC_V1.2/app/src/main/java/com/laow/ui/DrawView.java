package com.laow.ui;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.Rect;
import android.view.View;

/**
 * Created by linux on 2017/4/18.
 */

public class DrawView extends View {

    /*左环的圆心*/
    public float x = 240;
    public float y = 850;

    /*内外环半径*/
    public float r1 = 200;
    public float r2 = 100;
    public float r3 = 70;

    /*标志变量*/
    public int flag = -1;

    /*构造方法*/
    public DrawView(Context context) {
        super(context);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);

        Paint paint = new Paint();
        paint.setColor(Color.argb(255, 74, 173, 255));
        canvas.drawCircle(x, y, r1, paint);
        paint.setColor(Color.argb(255, 255, 255, 255));
        canvas.drawCircle(x, y, r2, paint);

        /*先让画布旋转才能画出旋转效果*/
        canvas.rotate(45, x, y);

        paint.setColor(Color.WHITE);
        paint.setStrokeWidth(8);
        canvas.drawLine(x - r1, y, x + r1, y, paint);
        canvas.drawLine(x, y + r1, x, y - r1, paint);
        canvas.restore();

        paint.setColor(Color.argb(255, 74, 173, 255));
        canvas.drawCircle(x, y, r3, paint);

        paint.setColor(Color.WHITE);
        paint.setTextSize(30);  /*pix*/
        String leftText = "左转";
        String rightText = "右转";
        String upText = "前进";
        String downText = "后退";
        String startText = "启动";
        String stopText = "停止";
        Rect rbound = new Rect();
        paint.getTextBounds(leftText, 0, leftText.length(), rbound);
        paint.getTextBounds(rightText, 0, rightText.length(), rbound);
        paint.getTextBounds(upText, 0, upText.length(), rbound);
        paint.getTextBounds(downText, 0, downText.length(), rbound);
        paint.getTextBounds(startText, 0, startText.length(), rbound);
        paint.getTextBounds(stopText, 0, stopText.length(), rbound);
        canvas.drawText(leftText, x - r1 + 10, y + rbound.height() / 2, paint);
        canvas.drawText(rightText, x + r1 - 70, y + rbound.height() / 2, paint);
        canvas.drawText(upText, x-25, y - r1 + 50, paint);
        canvas.drawText(downText, x-25, y + r1-25, paint);
        canvas.drawText(startText, x-25, y+15, paint);
        /*canvas.drawText(stopText, x-25, y+15, paint);*/
    }

    /*判断点是否落在圆环范围内*/
    public boolean isInCircle(float click_x, float click_y) {

        float distance = (float) Math.sqrt(((click_x - x) * (click_x - x)) + ((click_y - y) * (click_y - y)));

        if (distance<r1) {
            return true;
        }else{
            return false;
        }
    }


    /*判断在哪个区域*/
    public int whichEdge(float click_x, float click_y) {

        float distance = (float) Math.sqrt(((click_x-x)*(click_x-x)) + ((click_y-y)*(click_y-y)));
        float y2 = -click_x + (x + y);
        float y3 = click_x - (x - y);

        if (click_y > y3 && click_y < y2)
            flag = 0;// 在左侧范围
        if (click_y < y3 && click_y < y2)
            flag = 1;// 上边范围
        if (click_y < y3 && click_y > y2)
            flag = 2;// 右边范围
        if (click_y > y3 && click_y > y2)
            flag = 3;// 底部范围
        if(distance>0 && distance<r3)
            flag = 4;
        return flag;
    }
}
