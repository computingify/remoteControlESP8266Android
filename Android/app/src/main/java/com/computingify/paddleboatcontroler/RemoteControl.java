package com.computingify.paddleboatcontroler;

import android.os.Bundle;
import android.os.Message;

import androidx.appcompat.app.AppCompatActivity;

import io.github.controlwear.virtual.joystick.android.JoystickView;

public class RemoteControl extends AppCompatActivity {

    private final String url = "192.168.4.1";
    private final int port = 80;
    int oldOrder;
    private Communication communication;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_remote_control);

        final JoystickView joystickLeft = findViewById(R.id.joystickView_left);

        joystickLeft.setOnMoveListener((angle, strengthRcv) -> {
            int order = determineOrder(angle, strengthRcv);

            if (order != oldOrder) {
                oldOrder = order;
                Message msg = Message.obtain();
                msg.obj = (Integer.valueOf(order)).toString() + "#";
                communication.getHandler().sendMessage(msg);
            }
        }, 250);

        communication = new Communication(url, port);
    }

    public int determineOrder(int angle, int strength) {
        if (strength >= 70) {
            if (angle >= 337) {
                // Turn Right
                return message.TURN_RIGHT.ordinal();
            } else if (angle >= 292) {
                // Backward and Right
                return message.BACKWARD_TURN_RIGHT.ordinal();
            } else if (angle >= 247) {
                // Backward
                return message.BACKWARD.ordinal();
            } else if (angle >= 202) {
                // Backward and Left
                return message.BACKWARD_TURN_LEFT.ordinal();
            } else if (angle >= 157) {
                // Left Turn
                return message.TURN_LEFT.ordinal();
            } else if (angle >= 112) {
                // Forward and Left
                return message.FROWARD_TURN_LEFT.ordinal();
            } else if (angle >= 67) {
                // Forward
                return message.FROWARD.ordinal();
            } else if (angle >= 22) {
                // Forward and Right
                return message.FROWARD_TURN_RIGHT.ordinal();
            } else {
                // Turn Right
                return message.TURN_RIGHT.ordinal();
            }
        }
        // Stop
        return message.STOP.ordinal();
    }


    private enum message {
        STOP,
        FROWARD,
        BACKWARD,
        TURN_RIGHT,
        TURN_LEFT,
        FROWARD_TURN_RIGHT,
        FROWARD_TURN_LEFT,
        BACKWARD_TURN_RIGHT,
        BACKWARD_TURN_LEFT
    }

}