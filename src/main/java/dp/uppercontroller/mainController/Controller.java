package dp.uppercontroller.mainController;

import lombok.extern.slf4j.Slf4j;
import org.springframework.web.bind.annotation.*;

import javax.microedition.io.StreamConnection;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.charset.StandardCharsets;

@Slf4j
@RestController
@RequestMapping("/")
public class Controller {
    StreamConnection streamConnection;
    OutputStream outputStream;
    InputStream inputStream;
    String receivedData;

    @GetMapping("/move")
    public String moveInstruction(@RequestParam String order) throws IOException {
        try {
            order = new MoveController().sendData(order);
            outputStream.write(order.getBytes());
        }catch (Exception e){
            return "请连接蓝牙";
        }
        return "SUCCESS";
    }

    @GetMapping("/connect")
    public void getConnection() throws IOException {
        streamConnection = RemoteDeviceDiscovery.getStreamConnection();
        outputStream = streamConnection.openOutputStream();
        inputStream = streamConnection.openInputStream();
    }

    @GetMapping("/receive")
    public String receiveData() {
        if (inputStream == null) {
            return "蓝牙未连接，无法接收数据";
        }
        try {
            try {
                String order = new MoveController().sendData("!@");
                outputStream.write(order.getBytes());
            }catch (Exception e){
                return "请连接蓝牙";
            }

            byte[] buffer = new byte[1024]; // 设置缓冲区
            int bytesRead = inputStream.read(buffer); // 从输入流中读取数据

            if (bytesRead == -1) {
                return "没有接收到数据";
            }

            // 将字节数组转换为Hex字符串
            receivedData = bytesToDecimalString(buffer, bytesRead);
            return receivedData;
        } catch (IOException e) {
            return "接收数据时发生错误：" + e.getMessage();
        }
    }
    @GetMapping("/setWave")
    public String setWave(@RequestParam String order){
        try{
            order = new MoveController().sendData(order);
            outputStream.write(order.getBytes());
            return "SUCCESS";
        }catch (Exception e){
            return e.getMessage();
        }

    }

    private String bytesToDecimalString(byte[] byteArray, int length) {
        StringBuilder decimalString = new StringBuilder();
        for (int i = 0; i < length; i++) {
            int decimal = byteArray[i] & 0xFF; // 将字节转换为无符号整数
            decimalString.append(decimal);
            if (i < length - 1) {
                decimalString.append(" "); // 用空格分隔每个十进制数字
            }
        }
        return decimalString.toString();
    }


}
