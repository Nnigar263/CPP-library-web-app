const express = require('express');
const { exec } = require('child_process');
const path = require('path'); // Path module add kiya hai files dhoondne ke liye
const app = express();

app.use(express.json());
app.use(express.static(__dirname));

app.post('/api', (req, res) => {
    const { action, args } = req.body;
    
    // Quotes wrap karne se spaces handle ho jayenge
    const safeArgs = args.map(a => `"${String(a).replace(/"/g, '\\"')}"`).join(' ');

    // --- YAHAN UPDATE KIYA HAI ---
    // Agar cloud (Render) par hai toh "./main", agar tere laptop par hai toh "main.exe"
    const isWindows = process.platform === "win32";
    const cmd = isWindows 
        ? `main.exe ${action} ${safeArgs}` 
        : `./main ${action} ${safeArgs}`;
    // ------------------------------

    exec(cmd, (err, stdout) => {
        if (err) {
            console.error("C++ Exec Error:", err);
            return res.json({ success: false, message: "C++ File missing or crashed" });
        }
        try {
            res.send(stdout.trim());
        } catch (e) {
            res.json({ success: false, message: "JSON Parse Error", raw: stdout });
        }
    });
});

// --- YAHAN BHI UPDATE KIYA HAI ---
// Cloud pe 'process.env.PORT' use hota hai, local pe 3000
const PORT = process.env.PORT || 3000;
app.listen(PORT, () => console.log(`Server running at port ${PORT}`));