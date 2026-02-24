# PerfectlyImperfect GitHub Publisher
# This script automates the initialization and upload of your project.

Write-Host "--- PerfectlyImperfect GitHub Publisher ---" -ForegroundColor Cyan

# 1. Initialize Git if not already done
if (!(Test-Path .git)) {
    git init
    Write-Host "[+] Git initialized" -ForegroundColor Green
}

git branch -M main

# 2. Add files
git add .
git commit -m "Official release of PerfectlyImperfect App"

# 3. Ask for URL
$repoUrl = Read-Host "Paste your GitHub Repository URL (e.g., https://github.com/user/repo.git)"

if ($repoUrl -match "https://github.com/.*\.git") {
    # Remove old origin if exists
    git remote remove origin 2>$null
    
    # Add new origin
    git remote add origin $repoUrl
    
    Write-Host "[*] Pushing to GitHub..." -ForegroundColor Yellow
    git push -u origin main
    
    if ($LASTEXITCODE -eq 0) {
        Write-Host "--- SUCCESS! Project is now live on GitHub ---" -ForegroundColor Green
        Write-Host "You can view it at: $($repoUrl.Replace('.git', ''))"
    } else {
        Write-Host "--- Error pushing to GitHub. Make sure you are logged in (run 'gh auth login') ---" -ForegroundColor Red
    }
} else {
    Write-Host "Invalid GitHub URL provided." -ForegroundColor Red
}

Pause
