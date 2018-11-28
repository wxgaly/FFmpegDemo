package nova.android.ffmpegdemo

import android.os.Bundle
import android.os.Environment
import android.support.design.widget.Snackbar
import android.support.v7.app.AppCompatActivity
import android.util.Log
import android.view.Menu
import android.view.MenuItem
import android.view.SurfaceHolder
import android.view.View
import kotlinx.android.synthetic.main.activity_main.*
import kotlinx.android.synthetic.main.content_main.*
import nova.android.ffmpegdemo.util.FFmpegHelper

class MainActivity : AppCompatActivity(), View.OnClickListener {

    private val TAG = "MainActivity"
    val VIDEO_PATH = "${Environment.getExternalStorageDirectory().absolutePath}/test.mp4"

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        setSupportActionBar(toolbar)

        fab.setOnClickListener { view ->
            Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
                    .setAction("Action", null).show()
            sample_text.text = FFmpegHelper.getInstance().urlprotocolinfo()
        }

        // Example of a call to a native method
        sample_text.text = FFmpegHelper.getInstance().stringFromJNI()

        initView()

        initListener()
    }

    private fun initView() {
        surfaceView.holder.addCallback(callback)
    }

    inner class Callback : SurfaceHolder.Callback {
        override fun surfaceChanged(holder: SurfaceHolder?, format: Int, width: Int, height: Int) {
        }

        override fun surfaceDestroyed(holder: SurfaceHolder?) {
        }

        override fun surfaceCreated(holder: SurfaceHolder?) {
            Log.d(TAG, "surfaceCreated")
//            holder?.apply {
//                FFmpegHelper.getInstance().renderVideo(VIDEO_PATH, surface)
//            }
        }
    }

    private val callback = Callback()

    private fun initListener() {
        btn_codec.setOnClickListener(this)
        btn_filter.setOnClickListener(this)
        btn_format.setOnClickListener(this)
        btn_protocol.setOnClickListener(this)
        btn_videoinfo.setOnClickListener(this)
        btn_start.setOnClickListener(this)
    }

    override fun onClick(v: View?) {

        v?.let {
            when (it.id) {
                R.id.btn_codec -> sample_text.text = FFmpegHelper.getInstance().avcodecinfo()
                R.id.btn_filter -> sample_text.text = FFmpegHelper.getInstance().avfilterinfo()
                R.id.btn_format -> sample_text.text = FFmpegHelper.getInstance().avformatinfo()
                R.id.btn_protocol -> sample_text.text = FFmpegHelper.getInstance().urlprotocolinfo()
                R.id.btn_videoinfo -> {
                    sample_text.text = "${FFmpegHelper.getInstance().getVideoInfo(VIDEO_PATH)}--- +++  ${FFmpegHelper
                            .getInstance().getRotation(VIDEO_PATH)}"
                }
                R.id.btn_start -> {
                    Thread {
                        FFmpegHelper.getInstance().renderVideo(VIDEO_PATH, surfaceView.holder.surface)
                    }.start()

                }
                else -> Log.d(TAG, "click not")
            }
        }

    }


    override fun onCreateOptionsMenu(menu: Menu): Boolean {
        // Inflate the menu; this adds items to the action bar if it is present.
        menuInflater.inflate(R.menu.menu_main, menu)
        return true
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        return when (item.itemId) {
            R.id.action_settings -> true
            else -> super.onOptionsItemSelected(item)
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        surfaceView.holder.removeCallback(callback)
    }

}
