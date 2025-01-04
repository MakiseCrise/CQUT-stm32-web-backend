package dp.uppercontroller.mainController;

import org.springframework.context.annotation.Bean;

import javax.bluetooth.*;
import javax.microedition.io.Connector;
import javax.microedition.io.StreamConnection;
import java.io.IOException;
import java.io.OutputStream;
import java.util.HashSet;
import java.util.Set;
import java.util.Vector;

public class RemoteDeviceDiscovery {
    public static Set<RemoteDevice> devicesDiscovered = new HashSet<>();

    // 流连接
    private static StreamConnection streamConnection = null;
    public final static Vector<String> serviceFound = new Vector<>();

    final static Object serviceSearchCompletedEvent = new Object();
    final static Object inquiryCompletedEvent = new Object();
    /*
     *蓝牙发现监听器
     *  */
    private static DiscoveryListener listener = new DiscoveryListener() {
        public void inquiryCompleted(int discType) {
            System.out.println("#" + "搜索完成");
            synchronized (inquiryCompletedEvent) {
                inquiryCompletedEvent.notifyAll();
            }
        }

        /*
         * 设别发现
         * */
        @Override
        public void deviceDiscovered(RemoteDevice remoteDevice, DeviceClass deviceClass) {
            devicesDiscovered.add(remoteDevice);

            try {
                System.out.println("#发现设备" + remoteDevice.getFriendlyName(false)+"  Address = "+remoteDevice.getBluetoothAddress());

            } catch (IOException e) {
                e.printStackTrace();
            }

        }


        @Override
        public void servicesDiscovered(int transID, ServiceRecord[] servRecord) {
            for (int i = 0; i < servRecord.length; i++) {
                String url = servRecord[i].getConnectionURL(ServiceRecord.NOAUTHENTICATE_NOENCRYPT, false);
                if (url == null) {
                    continue;
                }
                serviceFound.add(url);
                DataElement serviceName = servRecord[i].getAttributeValue(0x0100);
                if (serviceName != null) {
                    System.out.println("service " + serviceName.getValue() + " found " + url);
                } else {
                    System.out.println("service found " + url);
                }
            }
            System.out.println("#" + "servicesDiscovered");
        }


        @Override
        public void serviceSearchCompleted(int arg0, int arg1) {
            System.out.println("#" + "serviceSearchCompleted");
            synchronized (serviceSearchCompletedEvent) {
                serviceSearchCompletedEvent.notifyAll();
            }
        }
    };
    /*
     *发现设备并打印
     */

    private static void findDevices() throws IOException, InterruptedException {
        devicesDiscovered.clear();

        synchronized (inquiryCompletedEvent) {
            LocalDevice ld = LocalDevice.getLocalDevice();
            System.out.println("#本机蓝牙名称:" + ld.getFriendlyName());

            boolean started = LocalDevice.getLocalDevice().getDiscoveryAgent().startInquiry(DiscoveryAgent.GIAC, listener);

            if (started) {
                System.out.println("#" + "等待搜索完成...");
                inquiryCompletedEvent.wait();
                LocalDevice.getLocalDevice().getDiscoveryAgent().cancelInquiry(listener);

                System.out.println("#发现设备数量：" + devicesDiscovered.size());
            }
        }
    }

    public static Set<RemoteDevice> getDevices() throws IOException, InterruptedException {
        findDevices();
        return devicesDiscovered;
    }

    public static String searchService(RemoteDevice btDevice, String serviceUUID) throws IOException, InterruptedException {
        UUID[] searchUuidSet = new UUID[]{new UUID(serviceUUID, false)};

        int[] attrIDs = new int[]{
                0x0100 // Service name
        };

        synchronized (serviceSearchCompletedEvent) {
            System.out.println("search services on " + btDevice.getBluetoothAddress() + " " + btDevice.getFriendlyName(false));
            LocalDevice.getLocalDevice().getDiscoveryAgent().searchServices(attrIDs, searchUuidSet, btDevice, listener);
            serviceSearchCompletedEvent.wait();
        }

        if (serviceFound.size() > 0) {
            return serviceFound.elementAt(0);
        } else {
            return "";
        }
    }

    /*
    main method
    * */
    public static StreamConnection getStreamConnection() throws IOException {
        streamConnection = (StreamConnection) Connector.open("btspp://" + "98DA600A8597"+":1;authenticate=false;encrypt=false;master=false");
        return streamConnection;
    }

    public static void main(String[] args) throws IOException, InterruptedException {
        //发现服务并打印控制台
        //findDevices();
        //假若已知手机蓝牙地址，直接连接，”：1“指定是蓝牙信道
        //streamConnection = (StreamConnection) Connector.open("btspp://" + "9C9234534F7FHD72" + ":1");
        try {
            System.out.println("连接" + "btpp://" + "98DA600A8597" + ":1;authenticate=false;encrypt=false;master=false");
            streamConnection = (StreamConnection) Connector.open("btspp://" + "98DA600A8597"+":1;authenticate=false;encrypt=false;master=false");
            //SUCCESS
            //OutputStream outputStream = streamConnection.openOutputStream();
            //Thread.sleep(5000);
            //outputStream.write("@W\r\n".getBytes());
            //SUCCESS
        }catch (Exception e){
            e.printStackTrace();
        }

    }

}

