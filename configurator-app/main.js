const { app, BrowserWindow } = require('electron');
const path = require('path');

// Disable Private Network Access (PNA) checks to allow connections to local IPs
app.commandLine.appendSwitch('disable-features', 'BlockInsecurePrivateNetworkRequests,PrivateNetworkAccessRespectPreflightResults,PrivateNetworkAccessSendPreflights');

function createWindow() {
  const win = new BrowserWindow({
    width: 1080,
    height: 850,
    minWidth: 800,
    minHeight: 600,
    title: "SimDash Configurator",
    backgroundColor: "#0b0f19",
    webPreferences: {
      nodeIntegration: false,
      contextIsolation: true,
      webSecurity: false // Disables CORS/PNA/Mixed Content checks completely
    }
  });

  // Load the web-tool.html file from the parent directory
  win.loadFile(path.join(__dirname, 'web-tool.html'));

  // Remove the default menu bar
  win.setMenu(null);
}

app.whenReady().then(() => {
  createWindow();

  app.on('activate', () => {
    if (BrowserWindow.getAllWindows().length === 0) {
      createWindow();
    }
  });
});

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') {
    app.quit();
  }
});
