from flask import Flask, request, jsonify, render_template_string
import joblib
import numpy as np
import os

app = Flask(__name__)

# 获取当前脚本所在目录
BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

# 加载模型
try:
    model = joblib.load(os.path.join(BASE_DIR, 'models', 'california_model.pkl'))
    scaler = joblib.load(os.path.join(BASE_DIR, 'models', 'california_scaler.pkl'))
    print("✅ 模型加载成功")
except Exception as e:
    print(f"❌ 模型加载失败: {e}")
    model = None
    scaler = None

# 特征名称
feature_names = ['MedInc', 'HouseAge', 'AveRooms', 'AveBedrms', 
                 'Population', 'AveOccup', 'Latitude', 'Longitude']

@app.route('/')
def home():
    """显示主页 - 简化版本"""
    html = '''
    <!DOCTYPE html>
    <html>
    <head>
        <meta charset="UTF-8">
        <title>加州房价预测器</title>
        <style>
            body { font-family: Arial, sans-serif; margin: 40px; background: #f0f2f5; }
            .container { max-width: 600px; margin: 0 auto; background: white; padding: 30px; border-radius: 10px; box-shadow: 0 2px 10px rgba(0,0,0,0.1); }
            h1 { color: #333; text-align: center; }
            .form-group { margin-bottom: 20px; }
            label { display: block; margin-bottom: 5px; font-weight: bold; }
            input { width: 100%; padding: 10px; border: 1px solid #ddd; border-radius: 5px; font-size: 16px; }
            button { background: #6c5ce7; color: white; border: none; padding: 12px 20px; border-radius: 5px; font-size: 16px; cursor: pointer; width: 100%; }
            button:hover { background: #5a4fcf; }
        </style>
    </head>
    <body>
        <div class="container">
            <h1>🏠 加州房价预测器</h1>
            <p>请输入以下8个特征值：</p>
            <form action="/predict" method="post">
    '''
    
    # 添加8个输入框
    features_info = {
        'MedInc': '收入中位数（万美元/年）',
        'HouseAge': '房屋年龄中位数（年）',
        'AveRooms': '平均房间数（间）',
        'AveBedrms': '平均卧室数（间）',
        'Population': '街区人口（人）',
        'AveOccup': '平均入住率（人/户）',
        'Latitude': '纬度（度）',
        'Longitude': '经度（度）'
    }
    
    for feature in feature_names:
        html += f'''
                <div class="form-group">
                    <label for="{feature}">{feature}: {features_info[feature]}</label>
                    <input type="number" step="0.001" name="{feature}" id="{feature}" required>
                </div>
        '''
    
    html += '''
                <button type="submit">预测房价</button>
            </form>
        </div>
    </body>
    </html>
    '''
    
    return html

@app.route('/predict', methods=['POST'])
def predict():
    """预测接口"""
    try:
        if model is None or scaler is None:
            return "模型未加载，请检查模型文件"
        
        # 获取表单数据
        features = []
        for feature in feature_names:
            value = float(request.form.get(feature, 0))
            features.append(value)
        
        # 预测
        features_array = np.array([features])
        features_scaled = scaler.transform(features_array)
        prediction = model.predict(features_scaled)[0]
        
        # 转换为美元
        price_dollars = prediction * 100000
        
        return f'''
        <!DOCTYPE html>
        <html>
        <head>
            <meta charset="UTF-8">
            <title>预测结果</title>
            <style>
                body {{ font-family: Arial; margin: 40px; }}
                .result {{ background: #e8f5e9; padding: 20px; border-radius: 10px; margin: 20px 0; }}
                .price {{ font-size: 2em; color: #2e7d32; font-weight: bold; }}
                a {{ display: inline-block; margin-top: 20px; padding: 10px 20px; background: #6c5ce7; color: white; text-decoration: none; border-radius: 5px; }}
            </style>
        </head>
        <body>
            <h1>预测结果</h1>
            <div class="result">
                <p class="price">${price_dollars:,.2f}</p>
                <p>约 {price_dollars/10000:.1f} 万美元</p>
                <p>原始预测值: {prediction:.4f} (单位: 10万美元)</p>
            </div>
            <a href="/">重新预测</a>
        </body>
        </html>
        '''
        
    except Exception as e:
        return f"预测错误: {str(e)}"

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=5000)