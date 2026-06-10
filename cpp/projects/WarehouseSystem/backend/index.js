const express = require('express');
const sql = require('mssql/msnodesqlv8');
const cors = require('cors');

const app = express();
app.use(cors());
app.use(express.json());

const dbConfig = {
    connectionString: 'Driver={ODBC Driver 17 for SQL Server};Server=localhost;Database=WarehouseDB;Trusted_Connection=yes;'
};

sql.connect(dbConfig).then(() => {
    console.log('✅ SQL Server Connected (数据库已连接)');
}).catch(err => console.error('❌ Database Connection Failed:', err));

// 查
app.get('/api/packages', async (req, res) => {
    try {
        const result = await sql.query('SELECT * FROM Packages ORDER BY CreateTime DESC');
        res.json(result.recordset);
    } catch (err) { res.status(500).send(err.message); }
});

// 增
app.post('/api/packages', async (req, res) => {
    const { trackingNumber, sender, receiver, location } = req.body;
    try {
        const request = new sql.Request();
        request.input('t', sql.NVarChar, trackingNumber);
        request.input('s', sql.NVarChar, sender);
        request.input('r', sql.NVarChar, receiver);
        request.input('l', sql.NVarChar, location);
        await request.query("INSERT INTO Packages (TrackingNumber, Sender, Receiver, Location) VALUES (@t, @s, @r, @l)");
        res.json({ message: 'Added' });
    } catch (err) { res.status(500).send(err.message); }
});

// 改
app.put('/api/packages/:id', async (req, res) => {
    const { id } = req.params;
    const { status, location } = req.body;
    try {
        const request = new sql.Request();
        request.input('id', sql.Int, id);
        request.input('status', sql.NVarChar, status);
        request.input('loc', sql.NVarChar, location);
        await request.query("UPDATE Packages SET Status = @status, Location = @loc WHERE Id = @id");
        res.json({ message: 'Updated' });
    } catch (err) { res.status(500).send(err.message); }
});

// 删
app.delete('/api/packages/:id', async (req, res) => {
    const { id } = req.params;
    try {
        const request = new sql.Request();
        request.input('id', sql.Int, id);
        await request.query("DELETE FROM Packages WHERE Id = @id");
        res.json({ message: 'Deleted' });
    } catch (err) { res.status(500).send(err.message); }
});

app.post('/api/login', async (req, res) => {
    const { username, password } = req.body;
    try {
        const request = new sql.Request();
        request.input('u', sql.NVarChar, username);
        request.input('p', sql.NVarChar, password);
        // 查询用户
        const result = await request.query("SELECT * FROM Users WHERE Username = @u AND Password = @p");
        
        if (result.recordset.length > 0) {
            // 登录成功，返回用户信息
            res.json({ success: true, user: result.recordset[0] });
        } else {
            res.status(401).json({ success: false, message: '账号或密码错误' });
        }
    } catch (err) {
        res.status(500).send(err.message);
    }
});

app.listen(3000, () => {
    console.log('🚀 Backend running on http://localhost:3000');
});