package nova.android.ffmpegdemo

import android.os.Bundle
import android.support.design.widget.Snackbar
import android.support.v7.app.AppCompatActivity
import android.util.Log
import android.view.Menu
import android.view.MenuItem
import android.view.View

import kotlinx.android.synthetic.main.activity_main.*
import kotlinx.android.synthetic.main.content_main.*
import nova.android.ffmpegdemo.util.FFmpegHelper

class MainActivity : AppCompatActivity(), View.OnClickListener {

    private val TAG = "MainActivity"

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

        initListener()
    }

    private fun initListener() {
        btn_codec.setOnClickListener(this)
        btn_filter.setOnClickListener(this)
        btn_format.setOnClickListener(this)
        btn_protocol.setOnClickListener(this)
    }

    override fun onClick(v: View?) {

        v?.let {
            when (it.id) {
                R.id.btn_codec -> sample_text.text = FFmpegHelper.getInstance().avcodecinfo()
                R.id.btn_filter -> sample_text.text = FFmpegHelper.getInstance().avfilterinfo()
                R.id.btn_format -> sample_text.text = FFmpegHelper.getInstance().avformatinfo()
                R.id.btn_protocol -> sample_text.text = FFmpegHelper.getInstance().urlprotocolinfo()
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

}
