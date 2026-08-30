from pathlib import Path
p=Path('/home/ubuntu/pane-browser-v3/PaneBrowser.cpp')
s=p.read_text()
s=s.replace('.site img{width:24px;height:24px;border-radius:4px;object-fit:contain}', '.siteicon{width:24px;height:24px;border-radius:50%;display:inline-grid;place-items:center;background:#5f6368;color:#fff;font-size:12px;font-weight:700;flex:none}', 2)
start=s.index('            html += L"<div class=\\"row\\"')
end=s.index('\n', start)
line=s[start:end]
img_start=line.index('L"</div><div class=\\"site\\"><img')
img_end=line.index('<div><button class=\\"title\\"', img_start)
replacement='L"</div><div class=\\"site\\"><span class=\\"siteicon\\">" + HtmlEscape(host.empty() ? L"?" : host.substr(0, 1)) + L"</span><div>'
line=line[:img_start]+replacement+line[img_end+len('<div>'):]
s=s[:start]+line+s[end:]
p.write_text(s)
print('history privacy patch applied')
