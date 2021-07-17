import java.net.*;
import java.util.*;
import java.io.*;
import java.text.*;
class IOTServer
{
private ServerSocket serverSocket;
private Socket socket;
private InputStream inputStream;
private OutputStream outputStream;
private InputStreamReader inputStreamReader;
private OutputStreamWriter outputStreamWriter;
private StringBuffer stringBuffer;
private String request,response;
private Date date;
private SimpleDateFormat simpleDateFormat;
public void start()
{
String splits[];
try
{
serverSocket=new ServerSocket(5050);
while(true)
{
System.out.println("Server is listening on port : 5050");
socket=serverSocket.accept();
stringBuffer=new StringBuffer();
inputStream=socket.getInputStream();
inputStreamReader=new InputStreamReader(inputStream);
int x;
while(true)
{
x=inputStreamReader.read();
if(x==-1 || x=='#') break;
stringBuffer.append((char)x);
}
request=stringBuffer.toString().trim();
System.out.println("Request arrived -> "+request);
splits=request.split(",");
int day=Integer.parseInt(splits[0]);
int month=Integer.parseInt(splits[1]);
int year=Integer.parseInt(splits[2]);
int hour=Integer.parseInt(splits[3]);
int minute=Integer.parseInt(splits[4]);
int second=Integer.parseInt(splits[5]);
date=new Date();
simpleDateFormat=new SimpleDateFormat("dd-MM-YYYY hh:mm:ss");
String currentDateTime[]=simpleDateFormat.format(date).split(" ");
String dateSplits[]=currentDateTime[0].split("-");
String timeSplits[]=currentDateTime[1].split(":");
response="";
if(Integer.parseInt(dateSplits[2])>year) 
{
System.out.println("Date & time updation required...");
response+=dateSplits[0]+","+dateSplits[1]+","+dateSplits[2]+","+timeSplits[0]+","+timeSplits[1]+","+timeSplits[2]+",2#";
outputStream=socket.getOutputStream();
outputStreamWriter=new OutputStreamWriter(outputStream);
outputStreamWriter.write(response);
outputStreamWriter.flush();
socket.close();
}
else if(Integer.parseInt(dateSplits[2])<=year)
{
System.out.println("Date & time updation not required...");
response+="1#";
outputStream=socket.getOutputStream();
outputStreamWriter=new OutputStreamWriter(outputStream);
outputStreamWriter.write(response);
outputStreamWriter.flush();
socket.close();
}
System.out.println("Response sent -> "+response);
}
}catch(Exception e)
{
System.out.println(e);
}
}
public static void main(String gg[])
{
IOTServer server=new IOTServer();
server.start();
}
}