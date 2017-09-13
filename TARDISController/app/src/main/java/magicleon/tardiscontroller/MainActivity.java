package magicleon.tardiscontroller;

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.support.v4.app.LoaderManager;
import android.support.v4.content.AsyncTaskLoader;
import android.support.v4.content.Loader;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Arrays;
import java.util.concurrent.TimeUnit;

import butterknife.BindView;
import butterknife.ButterKnife;

public class MainActivity extends AppCompatActivity implements SensorEventListener,LoaderManager.LoaderCallbacks {
    private SensorManager mSensorManager;
    private Sensor mAccelerometer;
    private Sensor mGyroscope;
    private double[] gravity = new double[3];
    static final double alpha = 0.8;
    private Socket socket;
    private OutputStream outputStream;
    private PrintWriter printWriter;
    private boolean canSend = true;

    private static final int LOADER_ID = 0;

    @BindView(R.id.accel_x) TextView accel_x_tv;
    @BindView(R.id.accel_y) TextView accel_y_tv;
    @BindView(R.id.accel_z) TextView accel_z_tv;
    @BindView(R.id.gyro_x)  TextView gyro_x_tv;
    @BindView(R.id.gyro_y)  TextView gyro_y_tv;
    @BindView(R.id.gyro_z)  TextView gyro_z_tv;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ButterKnife.bind(this);

        mSensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
        mAccelerometer = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        mGyroscope = mSensorManager.getDefaultSensor(Sensor.TYPE_GYROSCOPE);
        mSensorManager.registerListener(this,mAccelerometer,SensorManager.SENSOR_DELAY_NORMAL);
        mSensorManager.registerListener(this,mGyroscope,SensorManager.SENSOR_DELAY_NORMAL);

    }

    @Override
    protected void onResume() {
        super.onResume();
        mSensorManager.registerListener(this,mAccelerometer,SensorManager.SENSOR_DELAY_NORMAL);
        mSensorManager.registerListener(this,mGyroscope,SensorManager.SENSOR_DELAY_NORMAL);
    }

    @Override
    protected void onPause() {
        super.onPause();
        mSensorManager.unregisterListener(this);
    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        if (event.sensor.getType() == Sensor.TYPE_ACCELEROMETER){
            double[] linear_acceleration = new double[3];
            double x = event.values[0];
            double y = event.values[1];
            double z = event.values[2];

            // Isolate the force of gravity with the low-pass filter.
            gravity[0] = alpha * gravity[0] + (1 - alpha) * event.values[0];
            gravity[1] = alpha * gravity[1] + (1 - alpha) * event.values[1];
            gravity[2] = alpha * gravity[2] + (1 - alpha) * event.values[2];

            // Remove the gravity contribution with the high-pass filter.
            linear_acceleration[0] = event.values[0] - gravity[0];
            linear_acceleration[1] = event.values[1] - gravity[1];
            linear_acceleration[2] = event.values[2] - gravity[2];
            accel_x_tv.setText(Double.toString(linear_acceleration[0]));
            accel_y_tv.setText(Double.toString(linear_acceleration[1]));
            accel_z_tv.setText(Double.toString(linear_acceleration[2]));
            sendData(linear_acceleration);


        }
        if (event.sensor.getType() == Sensor.TYPE_GYROSCOPE){
            float x = event.values[0];
            float y = event.values[1];
            float z = event.values[2];
            gyro_x_tv.setText(Float.toString(x));
            gyro_y_tv.setText(Float.toString(y));
            gyro_z_tv.setText(Float.toString(z));

        }
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {

    }

    @Override
    public Loader onCreateLoader(int id, final Bundle args) {
        return new AsyncTaskLoader<Object>(this) {
            @Override
            protected void onStartLoading() {
                super.onStartLoading();
                forceLoad();
            }

            @Override
            public Void loadInBackground() {
                try{
                    double[] accel = args.getDoubleArray("DATA");
                    String data = Arrays.toString(accel);
                    socket = new Socket("192.168.1.83",1933);
                    outputStream = socket.getOutputStream();
                    printWriter = new PrintWriter(outputStream);
                    printWriter.println(data);
                    printWriter.flush();
                    printWriter.close();
                    socket.close();
                    canSend = true;

                } catch (IOException e) {
                    Log.d("AAA","ERROR CREATING SOCKET");
                    canSend = true;
                }
                return null;
            }

        };
    }

    @Override
    public void onLoadFinished(Loader loader, Object data) {
    }

    @Override
    public void onLoaderReset(Loader loader) {

    }

    void sendData(double[] data){
        Bundle args = new Bundle();
        args.putDoubleArray("DATA",data);

        if (canSend){
            canSend = false;
            getSupportLoaderManager().restartLoader(LOADER_ID,args,this);
        }
    }
}
