// 以下内容为无后端连接时书写的内容 
/*
function generateFakeSensorData(){  // 用来生成假数据
    return{
        temperature: (20 + Math.random() * 10).toFixed(1), // 保留 1 位小数 (toFixed(1))
        humidity: (40 + Math.random() *30).toFixed(1),
        pressure: (1000 + Math.random() *20).toFixed(1)
    };
}

function updateDisplay(data) {  // updateDisplay(data) 接收一个数据对象
    const container = document.getElementById("sensor-data"); // // document.getElementById("元素ID") 用来 通过元素的 id 获取 HTML 元素对象。
    container.innerHTML = `     
        <p> 温度：${data.temperature} °C</p>
        <p> 湿度：${data.humidity} %</p>
        <p> 气压：${data.pressure} hPa</p>
    `; // 模板字符串（`...`）更新 HTML 内容，显示最新数据   ${data.temperature} 等是 占位符，会显示数据对象里的值。
}

function loadSensorData(){
    const data = generateFakeSensorData();
    updateDisplay(data); // 调用 updateDisplay(data) 更新页面显示
}

document.getElementById("refresh-btn").addEventListener("click", loadSensorData); 
// 找到按钮 <button id="refresh-btn"> 给它绑定点击事件，当用户点击按钮时，执行 loadSensorData()，刷新显示数据。

window.onload = loadSensorData;
// 当页面加载完成时（window.onload），自动调用 loadSensorData() 这样用户一打开页面就能看到模拟数据，而不用先点按钮。

*/

async function fetchSensorData() {
    try{
        const response = await fetch('http://127.0.0.1:8000/sensor');
        // 通过 HTTP 向你的 FastAPI 后端发送一个 GET 请求，请求路径是 /sensor 
        // fetch() 是浏览器内置的 API，用来发网络请求；
        const data = await response.json();

        const sensor =  Array.isArray(data) ? data[0] : data;

        const container = document.getElementById("sensor-data");
        container.innerHTML = `
            <p>温度：${sensor.temperature} °C</p>
            <p>湿度：${sensor.humidity} %</p>
            <p>气压：${sensor.pressure} hPa</p>
        `;
    } catch (error) {
        console.error("获取数据失败：", error);
        alert("无法连接后端，请确认 FastAPI 已启动。");
    }
}

document.getElementById("refresh-btn").addEventListener("click", fetchSensorData);
// 找到按钮 <button id="refresh-btn"> 给它绑定一个“点击事件监听器” 当用户点击按钮时，调用 fetchSensorData() 再次从后端取数据。
window.onload = fetchSensorData;
// 当网页完全加载完后，自动执行一次 fetchSensorData()