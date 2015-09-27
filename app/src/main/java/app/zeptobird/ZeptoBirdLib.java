package app.zeptobird;

// Wrapper for native library

public class ZeptoBirdLib {

     static {
         System.loadLibrary("native_code");
     }

    /**
     * @param width the current view width
     * @param height the current view height
     */
     public static native void init(int width, int height);
     public static native void step();
     public static native void touch(float x, float y);
}
