const express = require('express');
const fs = require('fs');
const csv = require('csv-parser');
const bodyParser = require('body-parser');
const cors = require('cors');
const app = express();
const port = 7428;

// 使用中间件
app.use(cors());
app.use(bodyParser.json());
app.use(express.static('public'));

// 加载词典
let dictionary = {};

fs.createReadStream('dictionary.csv')
  .pipe(csv())
  .on('data', (row) => {
    dictionary[row.word.toLowerCase()] = row.translation;
  })
  .on('end', () => {
    console.log(`词典加载完成，包含 ${Object.keys(dictionary).length} 个词条`);
  });

// 查找相似单词的函数（改进版）
function findSimilarWords(target, maxSuggestions = 3) {
  const words = Object.keys(dictionary);
  const suggestions = [];
  
  target = target.toLowerCase();
  
  // 计算编辑距离
  function editDistance(a, b) {
    if (a.length === 0) return b.length;
    if (b.length === 0) return a.length;
    
    const matrix = [];
    for (let i = 0; i <= b.length; i++) matrix[i] = [i];
    for (let j = 0; j <= a.length; j++) matrix[0][j] = j;
    
    for (let i = 1; i <= b.length; i++) {
      for (let j = 1; j <= a.length; j++) {
        const cost = a[j - 1] === b[i - 1] ? 0 : 1;
        matrix[i][j] = Math.min(
          matrix[i - 1][j] + 1,
          matrix[i][j - 1] + 1,
          matrix[i - 1][j - 1] + cost
        );
      }
    }
    
    return matrix[b.length][a.length];
  }
  
  // 计算相似度得分
  const scoredWords = words.map(word => {
    const distance = editDistance(target, word);
    const lengthDiff = Math.abs(target.length - word.length);
    const score = distance + lengthDiff * 0.5;
    return { word, score };
  });
  
  // 排序并获取最相似的单词
  scoredWords.sort((a, b) => a.score - b.score);
  
  for (let i = 0; i < Math.min(maxSuggestions, scoredWords.length); i++) {
    if (scoredWords[i].score <= target.length + 2) { // 设置最大允许距离
      suggestions.push(scoredWords[i].word);
    }
  }
  
  return suggestions;
}

// 处理翻译请求
app.post('/translate', (req, res) => {
  const inputWords = req.body.words.split('\n').map(w => w.trim()).filter(w => w);
  
  const results = {
    translated: [],
    missed: []
  };
  
  inputWords.forEach(word => {
    const key = word.toLowerCase();
    if (dictionary[key]) {
      results.translated.push({
        word: word,
        translation: dictionary[key]
      });
    } else {
      const suggestions = findSimilarWords(word);
      results.missed.push({
        word: word,
        suggestions: suggestions
      });
    }
  });
  
  res.json(results);
});

// 导出CSV文件（只包含成功翻译的单词）
// 替换原来的/export路由
app.post('/export', (req, res) => {
    try {
        const { translated, missed } = req.body;
        
        let csvContent = "Word,Translation\n";
        
        // 处理翻译成功的单词
        translated.forEach(item => {
            csvContent += `"${item.word}","${item.translation.replace(/"/g, '""')}"\n`;
        });
        
        // 处理未找到的单词
        // missed.forEach(item => {
        //     const suggestions = item.suggestions ? item.suggestions.join('; ') : '';
        //     csvContent += `未找到,"${item.word}","","${suggestions.replace(/"/g, '""')}"\n`;
        // });
        
        res.setHeader('Content-Type', 'text/csv');
        res.setHeader('Content-Disposition', 'attachment; filename=translations.csv');
        res.send(csvContent);
        
    } catch (error) {
        console.error('导出处理错误:', error);
        res.status(500).json({ error: '导出处理失败' });
    }
});

app.listen(port, () => {
  console.log(`服务器运行在 http://localhost:${port}`);
});
