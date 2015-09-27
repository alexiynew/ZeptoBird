package app.zeptobird;

import android.app.Activity;
import android.os.Bundle;

public class ZeptoBirdActivity extends Activity {

    ZeptoBirdView mView;

    @Override protected void onCreate(Bundle icicle) {
        super.onCreate(icicle);
        mView = new ZeptoBirdView(getApplication());
	    setContentView(mView);
    }

    @Override protected void onPause() {
        super.onPause();
        mView.onPause();
    }

    @Override protected void onResume() {
        super.onResume();
        mView.onResume();
    }
}
