
// 导入所需的包
import javax.swing.*; // Swing GUI组件
import java.awt.*; // AWT组件
import java.awt.event.ActionEvent; // 处理用户界面操作的事件
import java.awt.event.ActionListener; // 监听事件处理
import java.awt.event.KeyEvent; // 键盘事件
import java.awt.event.KeyListener; // 键盘事件监听器
import java.io.DataInputStream; // 数据输入流
import java.io.DataOutputStream; // 数据输出流
import java.io.IOException; // 输入输出异常
import java.net.Socket; // 套接字
import java.net.SocketException; // 套接字异常
import java.awt.event.WindowAdapter; // 窗口适配器
import java.awt.event.WindowEvent; // 窗口事件

// 客户端主类，继承JFrame并实现KeyListener接口
public class ClientChar extends JFrame implements KeyListener {
    private boolean isConn = false; // 标记是否连接到服务器
    private String name; // 客户端用户名
    private boolean isAnonymous = false; // 是否选择匿名聊天模式

    // GUI组件声明
    private JTextArea jta; // 用于显示聊天信息的文本区域
    private JScrollPane jsp; // 滚动面板，用于显示文本区域
    private JPanel jp; // 面板，用于放置输入文本框和发送按钮
    private JTextField jtf; // 输入消息的文本框
    private JButton jb1; // 发送消息的按钮

    private Socket socket = null; // 套接字对象
    private static final String CONNSTR = "127.0.0.1"; // 连接服务器的IP地址
    private static final int CONNPORT = 8888; // 连接服务器的端口号
    private DataOutputStream dos = null; // 数据输出流

    // 客户端构造函数
    public ClientChar() {
        jta = new JTextArea(); // 创建文本区域
        jta.setEditable(false); // 设置文本区域不可编辑
        jsp = new JScrollPane(jta); // 将文本区域放入滚动面板
        jp = new JPanel(); // 创建面板
        jtf = new JTextField(15); // 创建文本输入框，参数为列数
        jb1 = new JButton("发送信息"); // 创建发送按钮
    }

    // 初始化方法，设置GUI界面和连接服务器
    public void init() {
        jta = new JTextArea(); // 创建文本区域
        jta.setEditable(false); // 设置文本区域不可编辑
        jsp = new JScrollPane(jta); // 将文本区域放入滚动面板
        jp = new JPanel(); // 创建面板
        jtf = new JTextField(15); // 创建文本输入框，参数为列数
        jb1 = new JButton("发送信息"); // 创建发送按钮

        jp.add(jtf); // 将文本输入框添加到面板
        jtf.addKeyListener(this); // 给文本输入框添加键盘监听器
        jp.add(jb1); // 将发送按钮添加到面板
        jb1.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String text = jtf.getText(); // 获取文本输入框中的文本
                if (text.startsWith("@@")) {
                    handleSystemCommand(text); // 处理系统命令
                } else {
                    sendMessage(text); // 发送普通消息
                }
                jtf.setText(""); // 清空文本输入框
            }
        });

        this.add(jsp, BorderLayout.CENTER); // 将滚动面板添加到窗口中间位置
        this.add(jp, BorderLayout.SOUTH); // 将面板添加到窗口底部位置

        this.setBounds(700, 300, 300, 300); // 设置窗口位置和大小
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE); // 设置窗口关闭操作
        this.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                try {
                    if (dos != null) {
                        dos.writeUTF("##exit"); // 发送退出命令给服务器
                        dos.flush();
                    }
                    if (socket != null) {
                        socket.close(); // 关闭套接字
                    }
                } catch (IOException ex) {
                    ex.printStackTrace();
                }
                System.exit(0); // 退出程序
            }
        });

        try {
            socket = new Socket(CONNSTR, CONNPORT); // 创建套接字连接服务器
            isConn = true; // 标记连接状态为true

            while (true) {
                name = JOptionPane.showInputDialog(this.getContentPane(), "请输入你的用户名:"); // 弹出对话框输入用户名
                String password = JOptionPane.showInputDialog(this.getContentPane(), "请输入你的密码:"); // 弹出对话框输入密码
                if (name.isEmpty() || password.isEmpty()) { // 检查用户名和密码是否为空
                    JOptionPane.showMessageDialog(this.getContentPane(), "用户名和密码不能为空，请重新输入。"); // 提示用户名和密码不能为空
                    continue; // 继续循环，等待重新输入
                }
                dos = new DataOutputStream(socket.getOutputStream()); // 获取套接字的输出流
                dos.writeUTF(name + "," + password); // 发送用户名和密码给服务器
                dos.flush(); // 刷新输出流

                DataInputStream dis = new DataInputStream(socket.getInputStream()); // 获取套接字的输入流
                String response = dis.readUTF(); // 读取服务器响应
                if (response.equals("success")) { // 如果服务器响应成功
                    break; // 跳出循环，登录成功
                } else {
                    JOptionPane.showMessageDialog(this.getContentPane(), "用户名或密码错误，请重新输入。"); // 提示用户名或密码错误
                }
            }

            this.setTitle("聊天室-" + name); // 设置窗口标题
            new Thread(new Receive()).start(); // 创建并启动接收消息的线程

            this.setVisible(true); // 设置窗口可见
        } catch (IOException e) {
            e.printStackTrace(); // 打印异常信息
        }
    }

    // 发送消息到服务器
    private void sendMessage(String str) {
        try {
            if (jtf.getText().length() == 0) {
                return; // 如果文本输入框为空，直接返回
            }

            if (str.startsWith("@")) { // 如果消息以"@"开头，表示私聊消息
                String[] parts = str.substring(1).split("：", 2); // 拆分消息，格式为@接收者：消息内容
                if (parts.length == 2) {
                    dos.writeUTF("@" + parts[0] + "：" + parts[1]); // 发送私聊消息给服务器
                    dos.flush(); // 刷新输出流
                } else {
                    jta.append("无效的私聊格式: " + str + "\n"); // 显示无效私聊格式提示
                }
            } else {
                dos.writeUTF(str); // 发送普通消息给服务器
                dos.flush(); // 刷新输出流
            }
        } catch (IOException e) {
            e.printStackTrace(); // 打印异常信息
        }
    }

    // 处理系统命令
    private void handleSystemCommand(String command) {
        try {
            if (command.equals("@@list")) { // 如果命令是@@list，请求获取在线用户列表
                dos.writeUTF(command); // 发送命令给服务器
                dos.flush(); // 刷新输出流
            } else if (command.equals("@@quit")) { // 如果命令是@@quit，请求退出聊天室
                dos.writeUTF(command); // 发送命令给服务器
                dos.flush(); // 刷新输出流
                socket.close(); // 关闭套接字
                System.exit(0); // 退出程序
            } else if (command.equals("@@showanonymous")) { // 如果命令是@@showanonymous，显示当前聊天方式
                jta.append("当前聊天方式为:" + (isAnonymous ? "匿名" : "实名") + "\n"); // 显示当前聊天方式
            } else if (command.equals("@@anonymous")) { // 如果命令是@@anonymous，切换聊天方式
                isAnonymous = !isAnonymous; // 切换匿名聊天模式状态
                dos.writeUTF(command); // 发送命令给服务器
                dos.flush(); // 刷新输出流
                jta.append("聊天方式已切换为:" + (isAnonymous ? "匿名" : "实名") + "\n"); // 显示切换后的聊天方式
            } else {
                jta.append("无效的系统命令，请重新输入\n"); // 显示无效系统命令提示
            }
        } catch (IOException e) {
            e.printStackTrace(); // 打印异常信息
        }
    }

    // 处理键盘按键按下事件
    @Override
    public void keyPressed(KeyEvent e) {
        if (e.getKeyCode() == KeyEvent.VK_ENTER) { // 如果按下的键是回车键
            String text = jtf.getText(); // 获取文本输入框中的文本
            if (text.startsWith("@@")) {
                handleSystemCommand(text); // 处理系统命令
            } else {
                sendMessage(text); // 发送普通消息
            }
            jtf.setText(""); // 清空文本输入框
        }
    }

    @Override
    public void keyReleased(KeyEvent e) {
    }

    @Override
    public void keyTyped(KeyEvent e) {
    }

    // 内部类，用于接收服务器发送的消息
    class Receive implements Runnable {
        @Override
        public void run() {
            try {
                while (isConn) { // 循环接收消息，直到连接断开
                    DataInputStream dis = new DataInputStream(socket.getInputStream()); // 获取套接字的输入流
                    String str = dis.readUTF(); // 读取服务器发送的消息
                    jta.append(str + "\n"); // 将消息显示在文本区域中
                }
            } catch (SocketException e) {
                jta.append("服务器终止" + System.lineSeparator()); // 显示服务器终止消息
            } catch (IOException e) {
                e.printStackTrace(); // 打印异常信息
            }
        }
    }

    // 主方法，程序入口
    public static void main(String[] args) throws IOException {
        new ClientChar().init(); // 创建客户端对象并初始化
    }
}
