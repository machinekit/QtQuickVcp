package io.machinekit.service;

import android.content.Context;
import android.net.wifi.WifiManager;
import android.net.wifi.WifiManager.MulticastLock;

public class MulticastActivator extends org.qtproject.qt5.android.bindings.QtActivity
{
    private static WifiManager.MulticastLock m_lock;
    private static MulticastActivator m_instance;

    public MulticastActivator()
    {
        m_instance = this;
    }

    public static void enable(Context context)
    {
        if (m_instance == null) {
            m_instance = new MulticastActivator();
        }

        WifiManager wifi = (WifiManager)context.getSystemService(Context.WIFI_SERVICE);
        if(wifi != null) {
            WifiManager.MulticastLock m_lock = wifi.createMulticastLock("Log_Tag");
            m_lock.acquire();
        }
    }

    public static void disable()
    {
        if (m_lock != null)
        {
            m_lock.release();
            m_lock = null;
        }
    }
}
