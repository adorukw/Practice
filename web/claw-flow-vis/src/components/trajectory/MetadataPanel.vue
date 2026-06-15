<script setup lang="ts">
import { computed, ref } from 'vue'
import { useTrajectoryStore } from '../../store/trajectory'

const store = useTrajectoryStore()
const collapsed = defineModel<boolean>('collapsed', { default: false })

type TabType = 'plugins' | 'skills' | 'prompting'
const activeTab = ref<TabType>('plugins')

const tabs: { key: TabType; label: string }[] = [
  { key: 'plugins', label: 'Plugins' },
  { key: 'skills', label: 'Skills' },
  { key: 'prompting', label: 'Prompting' },
]

const metadata = computed(() => store.metadata)

const plugins = computed(() => {
  const md = metadata.value
  if (!md?.plugins) return { imported: [] as string[], entries: [] as any[] }
  return {
    imported: md.plugins.importedRuntimePluginIds || [],
    entries: (md.plugins.entries || []).map((p: any) => ({
      id: p.id,
      name: p.name,
      version: p.version,
      description: p.description,
      activated: p.activated,
      explicitlyEnabled: p.explicitlyEnabled,
      activationSource: p.activationSource,
      status: p.status,
      origin: p.origin,
    })),
  }
})

const activatedPluginIds = computed(() =>
  new Set(plugins.value.entries.filter((p: any) => p.activated).map((p: any) => p.id))
)

const skills = computed(() => {
  const md = metadata.value
  if (!md?.skills) return []
  return (md.skills.entries || []).map((s: any) => ({
    id: s.id,
    name: s.name,
    description: s.description,
    available: s.available,
    invoked: s.invoked,
    source: s.source,
    filePath: s.filePath,
  }))
})

const invokedSkillIds = computed(() =>
  new Set(skills.value.filter((s: any) => s.invoked).map((s: any) => s.id))
)

const prompting = computed(() => {
  const md = metadata.value
  if (!md?.prompting) return null
  return {
    skillsPrompt: md.prompting.skillsPrompt || '',
    systemPromptReport: md.prompting.systemPromptReport || null,
  }
})

const injectedFiles = computed(() => {
  const report = prompting.value?.systemPromptReport
  if (!report) return []
  return report.injectedWorkspaceFiles || []
})

const promptTools = computed(() => {
  const report = prompting.value?.systemPromptReport
  if (!report) return []
  return report.tools?.entries || []
})

const promptSkills = computed(() => {
  const report = prompting.value?.systemPromptReport
  if (!report) return []
  return report.skills?.entries || []
})

const systemPromptStats = computed(() => {
  const report = prompting.value?.systemPromptReport
  if (!report) return null
  return {
    totalChars: report.systemPrompt?.chars || 0,
    projectChars: report.systemPrompt?.projectContextChars || 0,
    nonProjectChars: report.systemPrompt?.nonProjectContextChars || 0,
    skillsPromptChars: report.skills?.promptChars || 0,
    toolsSchemaChars: report.tools?.schemaChars || 0,
    bootstrapMaxChars: report.bootstrapMaxChars || 0,
    bootstrapTotalMaxChars: report.bootstrapTotalMaxChars || 0,
  }
})

const expandedSkill = ref<string | null>(null)
const expandedPlugin = ref<string | null>(null)
</script>

<template>
  <aside
    class="bg-white border-l border-gray-200 flex flex-col shrink-0 overflow-hidden transition-all duration-200 min-w-0"
    :class="collapsed ? 'w-10' : 'w-80'"
    :style="!collapsed ? 'max-width: 320px; width: 320px' : ''"
  >
    <div class="flex items-center justify-between px-3 py-2 border-b border-gray-100">
      <span v-if="!collapsed" class="text-xs font-semibold text-gray-400 uppercase tracking-wider">元数据</span>
      <button
        class="w-6 h-6 flex items-center justify-center rounded text-gray-400 hover:text-gray-600 hover:bg-gray-100 transition-colors text-xs"
        @click="collapsed = !collapsed"
      >
        {{ collapsed ? '<' : '>' }}
      </button>
    </div>

    <template v-if="!collapsed">
      <div class="p-3 border-b border-gray-100">
        <div class="flex gap-1">
          <button
            v-for="tab in tabs"
            :key="tab.key"
            class="flex-1 text-xs px-3 py-2 rounded-lg font-semibold transition-all duration-200"
            :class="activeTab === tab.key
              ? 'bg-gray-900 text-white shadow-sm'
              : 'bg-gray-50 text-gray-500 hover:bg-gray-100'"
            @click="activeTab = tab.key"
          >
            {{ tab.label }}
          </button>
        </div>
      </div>

      <div class="flex-1 overflow-y-auto overflow-x-hidden min-w-0">
        <!-- Plugins Tab -->
        <div v-if="activeTab === 'plugins'" class="p-4 space-y-4 min-w-0">
          <div class="bg-gray-50 rounded-lg p-3">
            <div class="text-xs text-gray-400 font-semibold mb-2">概览</div>
            <div class="grid grid-cols-2 gap-2 text-sm">
              <div>
                <span class="text-gray-500">注册插件</span>
                <span class="ml-2 font-bold text-gray-800">{{ plugins.imported.length }}</span>
              </div>
              <div>
                <span class="text-gray-500">已激活</span>
                <span class="ml-2 font-bold text-emerald-600">{{ activatedPluginIds.size }}</span>
              </div>
            </div>
          </div>

          <div>
            <div class="text-xs text-gray-400 font-semibold mb-2">已激活插件</div>
            <div class="space-y-2">
              <div
                v-for="plugin in plugins.entries.filter((p: any) => p.activated)"
                :key="plugin.id"
                class="bg-emerald-50 border border-emerald-200 rounded-lg p-3 cursor-pointer hover:bg-emerald-100 transition-colors"
                @click="expandedPlugin = expandedPlugin === plugin.id ? null : plugin.id"
              >
                <div class="flex items-center gap-2">
                  <span class="w-2 h-2 rounded-full bg-emerald-500 shrink-0"></span>
                  <span class="text-sm font-semibold text-gray-800">{{ plugin.name }}</span>
                  <span v-if="plugin.version" class="text-[10px] text-gray-400 bg-gray-100 px-1.5 py-0.5 rounded font-mono">v{{ plugin.version }}</span>
                  <span v-if="plugin.explicitlyEnabled" class="text-[10px] text-blue-600 bg-blue-50 px-1.5 py-0.5 rounded">显式启用</span>
                </div>
                <div v-if="expandedPlugin === plugin.id" class="mt-2 text-xs text-gray-500 space-y-1 break-words">
                  <div v-if="plugin.description">{{ plugin.description }}</div>
                  <div>来源: <span class="font-mono text-gray-600 break-all">{{ plugin.origin }}</span></div>
                  <div>激活源: <span class="font-mono text-gray-600 break-all">{{ plugin.activationSource }}</span></div>
                  <div>状态: <span class="font-mono text-gray-600">{{ plugin.status }}</span></div>
                </div>
              </div>
            </div>
          </div>

          <div>
            <div class="text-xs text-gray-400 font-semibold mb-2">未激活插件 ({{ plugins.imported.length - activatedPluginIds.size }})</div>
            <div class="flex flex-wrap gap-1.5">
              <span
                v-for="pid in plugins.imported.filter((id: string) => !activatedPluginIds.has(id))"
                :key="pid"
                class="text-xs text-gray-400 bg-gray-50 px-2 py-1 rounded border border-gray-100 break-all"
              >
                {{ pid }}
              </span>
            </div>
          </div>
        </div>

        <!-- Skills Tab -->
        <div v-if="activeTab === 'skills'" class="p-4 space-y-4 min-w-0">
          <div class="bg-gray-50 rounded-lg p-3">
            <div class="text-xs text-gray-400 font-semibold mb-2">概览</div>
            <div class="grid grid-cols-3 gap-2 text-sm">
              <div>
                <span class="text-gray-500">总数</span>
                <span class="ml-1 font-bold text-gray-800">{{ skills.length }}</span>
              </div>
              <div>
                <span class="text-gray-500">已调用</span>
                <span class="ml-1 font-bold text-amber-600">{{ invokedSkillIds.size }}</span>
              </div>
              <div>
                <span class="text-gray-500">可用</span>
                <span class="ml-1 font-bold text-emerald-600">{{ skills.filter((s: any) => s.available).length }}</span>
              </div>
            </div>
          </div>

          <div>
            <div class="text-xs text-gray-400 font-semibold mb-2">已调用 Skill</div>
            <div class="space-y-2">
              <div
                v-for="skill in skills.filter((s: any) => s.invoked)"
                :key="skill.id"
                class="bg-amber-50 border border-amber-200 rounded-lg p-3 cursor-pointer hover:bg-amber-100 transition-colors"
                @click="expandedSkill = expandedSkill === skill.id ? null : skill.id"
              >
                <div class="flex items-center gap-2">
                  <span class="w-2 h-2 rounded-full bg-amber-500 shrink-0"></span>
                  <span class="text-sm font-semibold text-gray-800">{{ skill.name }}</span>
                  <span class="text-[10px] text-amber-600 bg-amber-100 px-1.5 py-0.5 rounded">已调用</span>
                </div>
                <div v-if="expandedSkill === skill.id" class="mt-2 text-xs text-gray-500 space-y-1 break-words">
                  <div v-if="skill.description" class="leading-relaxed">{{ skill.description }}</div>
                  <div>来源: <span class="font-mono text-gray-600 break-all">{{ skill.source }}</span></div>
                </div>
              </div>
              <div v-if="skills.filter((s: any) => s.invoked).length === 0" class="text-sm text-gray-400 italic">
                本次会话未调用任何 Skill
              </div>
            </div>
          </div>

          <div>
            <div class="text-xs text-gray-400 font-semibold mb-2">可用但未调用 ({{ skills.filter((s: any) => s.available && !s.invoked).length }})</div>
            <div class="space-y-1.5">
              <div
                v-for="skill in skills.filter((s: any) => s.available && !s.invoked)"
                :key="skill.id"
                class="bg-gray-50 border border-gray-100 rounded-lg px-3 py-2 cursor-pointer hover:bg-gray-100 transition-colors"
                @click="expandedSkill = expandedSkill === skill.id ? null : skill.id"
              >
                <div class="flex items-center gap-2">
                  <span class="w-2 h-2 rounded-full bg-gray-300 shrink-0"></span>
                  <span class="text-sm text-gray-600">{{ skill.name }}</span>
                  <span class="text-[10px] text-gray-400 bg-gray-100 px-1.5 py-0.5 rounded">{{ skill.source }}</span>
                </div>
                <div v-if="expandedSkill === skill.id" class="mt-1.5 text-xs text-gray-500 leading-relaxed break-words">
                  {{ skill.description }}
                </div>
              </div>
            </div>
          </div>
        </div>

        <!-- Prompting Tab -->
        <div v-if="activeTab === 'prompting'" class="p-4 space-y-4 min-w-0">
          <div v-if="systemPromptStats" class="bg-gray-50 rounded-lg p-3">
            <div class="text-xs text-gray-400 font-semibold mb-2">System Prompt 统计</div>
            <div class="space-y-2">
              <div class="flex items-center justify-between text-sm">
                <span class="text-gray-500">总字符数</span>
                <span class="font-bold text-gray-800">{{ systemPromptStats.totalChars.toLocaleString() }}</span>
              </div>
              <div class="w-full bg-gray-200 rounded-full h-2">
                <div class="h-2 rounded-full bg-blue-500" :style="{ width: Math.min(100, (systemPromptStats.projectChars / systemPromptStats.totalChars) * 100) + '%' }"></div>
              </div>
              <div class="grid grid-cols-2 gap-2 text-xs">
                <div>
                  <span class="text-gray-400">项目上下文</span>
                  <span class="ml-1 font-mono text-blue-600">{{ systemPromptStats.projectChars.toLocaleString() }}</span>
                </div>
                <div>
                  <span class="text-gray-400">非项目上下文</span>
                  <span class="ml-1 font-mono text-gray-600">{{ systemPromptStats.nonProjectChars.toLocaleString() }}</span>
                </div>
                <div>
                  <span class="text-gray-400">Skills Prompt</span>
                  <span class="ml-1 font-mono text-amber-600">{{ systemPromptStats.skillsPromptChars.toLocaleString() }}</span>
                </div>
                <div>
                  <span class="text-gray-400">Tools Schema</span>
                  <span class="ml-1 font-mono text-emerald-600">{{ systemPromptStats.toolsSchemaChars.toLocaleString() }}</span>
                </div>
              </div>
              <div class="text-xs text-gray-400 pt-1 border-t border-gray-200">
                Bootstrap 上限: {{ systemPromptStats.bootstrapTotalMaxChars?.toLocaleString() }} 字符
              </div>
            </div>
          </div>

          <div>
            <div class="text-xs text-gray-400 font-semibold mb-2">注入的工作区文件</div>
            <div class="space-y-1.5">
              <div
                v-for="file in injectedFiles"
                :key="file.name"
                class="bg-gray-50 border border-gray-100 rounded-lg px-3 py-2"
                :class="{ 'opacity-50': file.missing }"
              >
                <div class="flex items-center gap-2 min-w-0">
                  <span class="w-2 h-2 rounded-full shrink-0" :class="file.missing ? 'bg-red-400' : 'bg-emerald-500'"></span>
                  <span class="text-sm font-mono text-gray-700 truncate">{{ file.name }}</span>
                  <span v-if="file.missing" class="text-[10px] text-red-500 bg-red-50 px-1.5 py-0.5 rounded">缺失</span>
                  <span v-if="file.truncated" class="text-[10px] text-amber-600 bg-amber-50 px-1.5 py-0.5 rounded">截断</span>
                </div>
                <div class="text-xs text-gray-400 mt-1 flex gap-3">
                  <span>原始: {{ file.rawChars?.toLocaleString() }} 字符</span>
                  <span>注入: {{ file.injectedChars?.toLocaleString() }} 字符</span>
                </div>
              </div>
            </div>
          </div>

          <div>
            <div class="text-xs text-gray-400 font-semibold mb-2">注入的工具 ({{ promptTools.length }})</div>
            <div class="flex flex-wrap gap-1.5 min-w-0">
              <span
                v-for="tool in promptTools"
                :key="tool.name"
                class="text-xs bg-emerald-50 text-emerald-700 border border-emerald-200 px-2 py-1 rounded font-mono break-all"
              >
                {{ tool.name }}
                <span class="text-emerald-400 ml-0.5">({{ tool.schemaChars }}c)</span>
              </span>
            </div>
          </div>

          <div>
            <div class="text-xs text-gray-400 font-semibold mb-2">Prompt 中的 Skill 声明 ({{ promptSkills.length }})</div>
            <div class="space-y-1">
              <div
                v-for="ps in promptSkills"
                :key="ps.name"
                class="flex items-center gap-2 text-xs min-w-0"
              >
                <span
                  class="w-2 h-2 rounded-full shrink-0"
                  :class="invokedSkillIds.has(ps.name) ? 'bg-amber-500' : 'bg-gray-300'"
                ></span>
                <span class="font-mono text-gray-600 truncate">{{ ps.name }}</span>
                <span class="text-gray-400">{{ ps.blockChars }}c</span>
                <span v-if="invokedSkillIds.has(ps.name)" class="text-amber-600 bg-amber-50 px-1.5 py-0.5 rounded">已调用</span>
              </div>
            </div>
          </div>
        </div>
      </div>
    </template>
  </aside>
</template>
