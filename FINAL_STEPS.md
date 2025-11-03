# Final Steps - Connect Your Project to GitLab

## Step 1: Get Your SSH URL from GitLab

1. Go to your project page: **simple-unix-shell**
2. Look for the **blue "Clone" button** near the top of the page
3. Click it to see options
4. Make sure **"Clone with SSH"** is selected (not HTTPS)
5. You'll see a URL like:
   ```
   git@gitlab.imt-atlantique.fr:YOUR_USERNAME/simple-unix-shell.git
   ```
6. **Copy this entire URL** (click the 📋 copy button)

## Step 2: Connect Your Project

Run this in WSL:

```bash
cd "/mnt/c/Users/lenovo/OS project/processes-shell"

# Remove the old template remote
git remote remove origin

# Add your private repository (REPLACE with your SSH URL from Step 1)
git remote add origin git@gitlab.imt-atlantique.fr:YOUR_USERNAME/simple-unix-shell.git

# Verify it's set correctly
git remote -v
```

## Step 3: Commit and Push Your Code

```bash
# Add all your files (including the new ones we created)
git add .

# Commit everything
git commit -m "Initial commit: Unix shell project setup"

# Push to your private repository
git push -u origin main
```

## Step 4: Verify on GitLab

1. Refresh your GitLab project page
2. You should see all your files there!
3. Your group member can now clone this repository

## Troubleshooting

**"Permission denied" when pushing:**
- Make sure SSH key is added to GitLab
- Test connection: `ssh -T git@gitlab.imt-atlantique.fr`

**"Repository not found":**
- Double-check the SSH URL format
- Make sure your username/project name is correct

**"Updates were rejected":**
- This shouldn't happen with a fresh repository
- If it does, you might need: `git push -u origin main --force`
- (But only if you're sure!)

---

**After this, you're all set to collaborate!** 🎉

