# 🏥 PSE em Ação — Sistema de Planejamento e Acompanhamento de Ações de Saúde na Escola

Repositório oficial referente à **Atividade de Estudo Programada (AEP) — 2º Semestre de 2026** do curso de **Análise e Desenvolvimento de Sistemas (ADS)** da **Universidade Cesumar (UniCesumar)**.

---

## 👨‍💻 Integrantes do Grupo
* **Eduardo Barbosa Montagnoli** — RA: 24150417-2
* **Thiago Vinicius Caldeira Lopes** — RA: 26013297-2
* **Vitor Augusto Aguilar** — RA: 26007539-2

---

## 🎯 Sobre o Projeto
O **PSE em Ação (IntegraPSE)** é uma aplicação executada em terminal, desenvolvida em **Linguagem C**, criada para auxiliar equipes escolares e profissionais da Atenção Primária à Saúde no planejamento, registro e acompanhamento estatístico de ações coletivas do **Programa Saúde na Escola (PSE)** (Decreto nº 6.286/2007).

Em conformidade com os limites éticos e de privacidade estudantil, o sistema opera exclusivamente com **dados fictícios e informações coletivas**, sem realizar diagnósticos clínicos ou armazenar dados sensíveis individuais de estudantes.

---

## ⚙️ Funcionalidades Implementadas
1. **Cadastrar ação do PSE:** Registro contendo código único, escola, tema, data prevista, público-alvo, responsável, quantidade prevista de participantes e situação inicial.
2. **Listar todas as ações:** Exibição organizada e formatada de todos os registros armazenados em memória.
3. **Pesquisar ações:** Localização rápida de registros filtrando por **Código**, **Escola** ou **Tema**.
4. **Atualizar situação:** Alteração do status da ação para *Planejada*, *Realizada* ou *Cancelada*, registrando a quantidade efetiva de participantes quando *Realizada*.
5. **Gerar resumo geral:** Painel consolidado apresentando o total de ações por situação, o total de participantes previstos e efetivos, e o percentual de participação.
6. **Validações de entrada:** Tratamento contra códigos duplicados, quantidades negativas ou nulas, campos obrigatórios vazios, opções inexistentes no menu e limite de armazenamento.

---

## 🚀 Instruções de Compilação e Execução

1. Clone este repositório:
   `git clone https://github.com/montagnoliedu/pse-em-acao-aep.git`

2. Compile o código-fonte utilizando o GCC:
   `gcc src/main.c -o pse_em_acao`

3. Execute a aplicação:
   * **No Windows:** `pse_em_acao.exe`
   * **No Linux / macOS:** `./pse_em_acao`
