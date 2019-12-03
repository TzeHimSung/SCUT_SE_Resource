## 软件架构重点归纳总结

### Useful architectural structures 主要架构结构

在书上第15页。

<img src="/home/jhseng/桌面/software_architecture_summary/p15.png"  />

主要分三类：Module 模块 / C&C 组件和连接件 / Allocation 分配

### 为什么软件架构很重要

在书上第25页。

1. Influence quality attributes 影响质量属性
2. Help reason about and manage change as the system evolves 系统的发展有助于改变和管理变化的原因
3. Early prediction of a system's qualities 一个系统其质量的早期预测
4. Enhances communication among stakeholders 加强涉众沟通
5. Capture the earliest and hence most fundamental, hardest-to-change design decisions 捕捉最早最根本最难以改变的设计决策
6. Defines a set of constraints on subsequent implementation 定义了一组在随后执行上的约束
7. Dictates the structure of an organization, or vice versa 决定一个组织的结构，反之亦然
8. Provide the basis for evolutionary prototyping 为进化原型的设计提供依据
9. Allows the architect and project manager to reason about cost and schedule 让架构师和项目经理对成本和进度追根溯源
10. As a transferable, reusable model that from the heart of a product line 作为一种可转让可重用的模型是产品线的核心
11. Architecture-based development focuses attention on the assembly of components, rather then simply on their creation 以架构为基础的发展重点关注组件的部件，而不是他们的创建过程
12. Reducing design and system complexity 减少设计和系统的复杂度
13. Be the foundation for training a new team member 为培训新的团队成员奠定基础

### 创建软件架构的7个主要活动

在书上第45页。这7个活动与选择的架构和life-cycle model毫无关系，是必须经历的过程。

- Making a business case for the system 为系统制定业务案例
- Understanding the architecturally significant requirements 了解架构重要要求
- Creating or selecting the architecture 创建或选择架构
- Documenting and communicating the architecture 交流和记录架构
- Analyzing or evaluating the architecture 分析或评估架构
- Implementing and testing the system based on the architecture 基于架构实现并测试系统
- Ensuring that the implementation conforms to the architecture 确保实现符合架构

### 架构设计决策7个类别

在书上第73页。

1. Allocation of responsibilities 分配职责
2. Coordination model 协调模式
3. Data model 数据模型
4. Management of resources 资源管理
5. Mapping among architectural elements 架构元素之间的映射
6. Binding time decisions 绑定时间思想
7. Choice of technology 技术选择

### Availability 可用性

待续

### Interoperability 互操作性

SOAP(simple object access protocol)和ReST(representation state transfer)是这章的重点。

#### SOAP与REST

如果想让web应用互操作，有两种现成的技术可以使用：SOAP和REST。

SOAP是针对基于XML的信息的协议。分布式应用程序可以使用它来交换信息，从而实现互操作。它常伴随着一组SOA中间件互操作标准和他们的合理实现，称为web服务。SOAP定义了如下标准：

- 组合服务的基础架构。
- 传输标准。
- 服务发现。使用UDDI语言来发布、监听服务。
- 可靠性。SOAP本身不保证信息可靠传输。需要信息可靠传输的应用程序必须使用符合SOAP可靠性标准的服务，比如WS-Reliability。

SOAP非常通用，它的根源是交互应用程序的远程调用过程模型。SOAP具有简单的类型系统。SOAP依靠HTTP和RPC进行信息传输，但从理论上讲可以在任何传输协议上实现。SOAP并不要求服务的方法名称、寻址模型或过程约定，因此使用SOAP几乎不会在应用程序之间带来实际的互操作性，这只是一套信息交换标准。交互的应用程序需要就如何处理有效负载达成共识，这就是获得语义互操作性的地方。

REST基于客户端-服务器的体系结构，围绕一小组“创建、读取、更新、删除”(CRUD)操作(或者叫post, get, put, delete)和单地址寻址方案进行构造。REST对体系结构没有什么限制。与它相比，SOAP提供了完整性，而REST提供了简单性。

REST是关于状态和状态转移的，它将web服务视为一个巨大的信息网络，可通过单地址URI寻址方案进行访问。REST中没有类型的概念，因此也没有类型检查，这取决于应用程序是否正确获取交互语义。

由于REST接口简单通用，因此任何HTTP客户端都可以使用REST操作与任何HTTP服务器进行通信，而无需进一步配置，这带来了语法上的互操作性。但是，对于这些程序的实际作用以及它们交换的信息，必须在组织级别达成一致，也就是说，如果仅两个服务都有REST接口，就不能保证服务之间的语义互操作性。

在HTTP之上的REST旨在进行自我描述，并且在最佳情况下是无状态协议。

在SOAP和REST之间进行选择的一个方面是，是否要接受SOAP+WSDL的复杂性和限制以获得更标准化的互操作性，或者是否想通过使用REST来避免开销，也许会受益于标准化程度的降低。

REST中的信息交换比SOAP中的信息交换具有更少的字符。因此，在REST和SOAP之间进行选择的权衡之一就是各个信息的大小。对于大量交换信息的系统，另一个折衷是性能(选REST)和结构化信息(选SOAP)之间的权衡。

选择SOAP或REST将取决于以下方面。例如所需的服务质量，SOAP对安全性、可用性以及功能类型有更大的支持。REST由于其简单性，更适合于典型的只读功能，比如mashup，这种情况对服务的质量要求较低。

### Modifiability 可修改性

待续

### Performance 性能

待续

### Security 安全性

表征安全性有三个特征：机密性、完整性和可用性。机密性是保护数据或服务免遭未经授权访问的属性。完整性是数据或服务不受未经授权操作的属性。可用性是系统可用于合法使用的属性。除此之外还有认证性、不可否认性和授权性三个属性。

### Testability 可测试性

待续

### Useability 易用性

设计清单(书p182)要看一下

### 其他质量属性

包括variability(可变性), portability(可移植性), development distributability(开发分布性), scalability(可伸缩性), deployability(可部署性), mobility(移动性), monitorability(可监视性), safety(安全性)

### 架构战术与模式(书Chap.13，超级重点)

架构模式是在实践中反复出现的一整套设计决策，它具有已知的属性，可以重复使用，且描述了一类架构。架构不是“发明”出来的，而是“发现”出来的。所以永远不会有完整的架构模式列表，一旦某些条件发生变化就会产生新的架构模式。经验丰富的架构师通常将创建架构视为选择、定制和组合架构模式的过程。与架构模式相比，战术作用范围更小，但更精细。可以说，战术是原子，而架构模式是分子。所以可以说：模式对战术进行了打包(整合)。

架构模式与以下成员建立联系：

- 语境(上下文)。产生问题的环境。

- 问题。架构模式概述了问题及其实体，并描述了任何互补或相反的作用力。问题的概述通常包含会遇到的质量属性。

- 解决方案。解决方案是成功解决问题并进行适当抽象的架构，描述了解决问题的体系结构，包括如何平衡工作中的多种力量，描述元素之间的职责和静态关系(使用模块结构)，或者描述元素的运行时行为以及元素之间的交互(C2C结构或分配结构)。解决方案由以下方式确定和描述：

  - 一系列元素类型。比如数据储存库，流程和对象。
  - 一系列交互机制和连接器。比如方法调用，事件和信息巴士。
  - 关于组件的拓扑布局。
  - 一组语义约束。它包含拓扑，元素行为和交互机制。

  解决方案应解释清楚元素的静态关系和运行时行为提供了何种质量属性。像下面这样。

  - Overview: ...
  - Elements: ...
  - Relations: ...
  - Constraints: ...
  - Weaknesses: ...

复杂的系统展现出多种模式。基于web的系统可能会采用三层C/S架构模式，在此架构模式下可能还会使用复制、代理、缓存、防火墙、MVC等，每一种架构模式都可能会采用更多的模式和策略。

#### Module Patterns 模块架构模式

##### Layered Pattern 层次模式

上下文：所有复杂的系统都需要独立开发和演化系统的各个部分。 由于这个原因，系统的开发人员需要明确且有据可查的关注点分离，以便可以独立开发和维护系统的模块。

问题：需要对软件分段，使得模块可以单独开发，而部件之间几乎没有交互，支持可移植性，可修改性和重用。

解决方案：将软件以层为单位划分。每个层为一组模块，提供一组连贯的服务。使用方向必须为单向(称为allowed-to-use关系，其实就是depends-on关系的专业表述)。每层通过公共接口公开出去。

约束：

- 软件的每一段必须归属于某一层
- 至少要有两层
- allowed-to-use关系不能成环

弱点：

- 增加层会增加系统的前期成本和复杂性
- 每一层对性能都有负面影响

#### Component-and-Connector Patterns 组件-连接件架构模式

##### Broker Pattern 代理(经纪人)模式

上下文：许多系统被一系列服务构件，并部署在多台服务器上。

问题：如何构建分布式软件以便服务用户，满足不需要知道服务提供商的的性质和位置就可以容易地动态更改用户和提供商之间的绑定？

解决方案：代理模式通过插入一个称为代理的中介，将服务的用户(客户端)与服务的提供者(服务器)分离。当客户端需要服务时，它对代理提供的服务接口发送请求。代理把该请求转发到服务器。

关系：附属关系将客户端和服务器与代理相关联。

约束：客户端和服务端都只能与代理交互。

弱点：

- 代理在客户端和服务器之间添加了一个间接层，因此增加了延迟，并且该层可能是通信瓶颈。
- 代理可能出现单点故障。
- 代理增加了前期的复杂性。
- 代理可能是安全攻击的目标。
- 代理可能很难测试。

##### Model-View-Controller Pattern MVC架构模式

上下文：把视图与系统的其余部分分开。

问题：如何将用户界面功能与应用程序功能分开，但仍然响应用户输入或低层应用程序数据的更改？当底层应用程序数据发生变化时，如何创建、维护和协调用户界面的多个视图？

解决方案：使用MVC模式将应用程序功能分为三种类型的组件：

- Model，一个模型，存放应用程序数据。
- View，一个视图，展示部分数据并与用户交互。
- Controller，一个控制器，在模型和视图之间进行中介，并管理状态更改的通知。

MVC不是对所有情况都适用。

关系：通知关系。连接模型、视图和控制器的实例，并通知相关状态更改元素。

约束：MVC至少各有一个实例。

弱点：

- 对于简单用户接口，复杂度可能会很高
- MVC抽象形式可能不能与一些用户接口工具包良好匹配。

##### Pipe-and-Filter Patter 管道和过滤器架构模式

上下文：许多系统要求从输入到输出传输离散数据流，许多类型的传输手段重复发生。

问题：系统需要被分成可重复使用的松散耦合的组件，这些组件具有简单的通用交互机制。

解决方案：使用管道-过滤器模式，其交互模式的特征在于数据流的连续变换。数据到达过滤器的输入端口，进行转换，然后通过管道的输出端口通过管道传递到下一个过滤器。 一个信号过滤器可以从一个或多个端口消耗数据或向一个或多个端口产生数据。

关系：附件关系将过滤器的输出与管道的输入关联，反之亦然。

约束：

- 管道将过滤器输出端口连接到过滤器输入端口。
- 连接的过滤器必须就通过连接管道传递的数据类型达成共识。
- 模式的特殊化可能会将组件的关联限制为无环图或线性序列，有时也称为管线。
- 其他特殊化可能规定组件具有某些命名的端口，例如UNIX过滤器的stdin，stdout和stderr端口。

弱点：

- 管道和过滤器模式通常不是交互式系统的理想选择。
- 拥有大量独立的过滤器会增加大量的计算开销。
- 管道和过滤器系统可能不适用于长时间运行的计算。

##### Client-Server Pattern C/S架构模式

上下文：有大量分布式客户端希望访问的共享资源和服务。

问题：我们希望通过集中控制这些资源和服务来提高可伸缩性和可用性，同事将资源本身分布在多个物理服务器上。

解决方案：客户端通过请求服务器的服务进行交互，服务器提供一系列服务。

关系：客户端和服务端是附属关系。

约束：

- 客户端和服务器通过请求/响应连接件连接
- 服务端可以充当其他服务端的客户端
- 专业化时可能添加约束条件
  - 给定端口的附件数量
  - 服务端允许的关系
- 组件可以按层排列，这些层是相关功能或将共享主机计算环境的功能的逻辑分组。

弱点：

- 服务端可能是性能瓶颈
- 服务端可能出现单点故障
- 一个系统建立后，决定每部分的功能通常是复杂的且花费很大。

##### Peer-to-Peer Pattern P2P架构模式

上下文：分布式计算实体被认为是对等的。

问题：一组对等的分布式计算实体如何通过公共协议相互连接，以便它们能够以高可用性和可扩展性组织和共享其服务？

解决方案：在P2P模式中，组件直接作为对等体进行交互。所有对等体都是“相等的”，并且没有对等体或对等体组对于系统的健康是至关重要的。

关系：附属关系将对等方与其连接器相关联。 附属关系可能会在运行时更改。

约束：

- 任何给定对等方允许的依赖数
- 用于搜索对等体的跳数
- 哪些对等体知道哪些对等体

有些P2P网络构造成星形，每个节点只能知道其父节点。

弱点：

- 管理安全性，数据一致性，数据(服务)可用性，备份和恢复都很复杂
- 小的P2P系统可能不能满足性能和可用性这两个质量属性

##### Service-Oriented Architecture Pattern 面向服务的(SOA)架构模式

上下文：许多服务互操作，但是对他们的实现没有任何详细的了解。

问题：如何支持在不同平台上运行并以不同实现语言编写的、由不同组织提供并分布在因特网上的分布式组件的互操作性？

解决方案：面向服务的体系结构(SOA)模式描述了提供和/或使用服务的分布式组件的集合。在SOA中，提供和服务使用者组件的服务提供者组件可以使用不同的实现语言和平台。 服务在很大程度上是独立的：服务提供商和服务使用者通常是独立部署的，并且通常属于不同的系统甚至不同的组织。 组件具有描述他们从其他组件请求的服务以及它们提供的服务的接口。

SOA连接件的基本类型包括SOAP、REST、asynchronous messaging(异步消息传递)。

关系：将各种类型的组件连接到各个连接器。

约束：服务使用者连接到服务提供商，但是可以使用中间组件。

弱点：

- 基于SOA的系统通常构建起来很复杂。
- 无法控制独立服务的发展。
- 与中间件相关联的性能开销很大，服务可能是性能瓶颈，通常不提供性能保证。

##### Publish-Subscribe Pattern 发布-订阅架构模式

上下文：数据生产者和消费者的确切数量和性质不是预先确定的或固定的，也不是它们共享的数据。

问题：如何创建支持在生产中和消费者之间传输消息的能力的集成机制，使他们互不知道对方的身份甚至存在？

解决方案：在发布-订阅模式中，组件通过已发布的消息或事件进行交互。

关系：附属关系通过规定哪些组件宣布事件以及哪些组件已注册以接收事件，从而将组件与发布-订阅连接器相关联。

约束：

- 所有组件都连接到事件分配器，该事件分配器可以看作是总线连接器或组件。 附加了发布端口以声明角色，并附加了订阅端口以监听角色。 约束可能会限制哪些组件可以侦听哪些事件，某个组件是否可以侦听其自己的事件以及系统中可以存在多少个发布-订阅连接器。
- 同时具有两种类型的端口，组件可以既是发布者又是订阅者。

弱点：

- 通常会增加等待时间，并对消息传递时间的可伸缩性和可预测性产生负面影响。
- 对消息顺序的控制较少，不能保证消息的传递。

##### Shared-Data Pattern 共享数据模式

上下文：各种计算组件需要共享和操作大量的数据。

问题：系统如何存储和操作由多个独立组件访问的持久数据？

解决方案：在共享数据模式中，交互主要由多个数据访问器和至少一个共享数据存储之间的持久数据交换所主导。

关系：附属关系确定哪些数据访问器连接到哪些数据存储。

约束：数据访问器与数据存储交互。

弱点：

- 共享数据存储可能成为性能瓶颈。
- 共享数据存储可能发生单点故障。
- 数据生产者和消费者可能紧密成对。

#### Allocation Patterns 分配架构模式

##### Map-Reduce Pattern 映射-减少模式

上下文：业务部门迫切需要以PB级快速分析其生成或访问的大量数据。 示例包括社交网站中的交互日志，大量文档或数据存储库以及搜索引擎的成对的<source，target> Web链接。 用于分析此数据的程序应易于编写，高效运行，并且在硬件故障方面具有弹性。

问题：有效地执行大型数据集的分布式并行排序，并为程序员指定要执行的分析提供了一种简单的方法。

解决方案：map-reduce模式需要三个部分：一个负责根据需要分配数据的专用基础设施；一个用于过滤数据以检索项目的map；一个结合了映射结果的reduce。

关系：”部署于“是映射-减少的实例与其所安装的处理器之间的关系。实例化，监视和控制是基础结构与映射-减少实例之间的关系。

约束：

- 被分析的数据必须以文件的形式保存
- 映射函数必须是状态无关的且不能互相沟通
- 映射实例与削减实例的唯一沟通是从映射实例发出的<key,value>键值对。

弱点：

- 如果没有大数据集，就没有理由使用映射-减少模式
- 如果没有把大数据集分割成若干个小的子集，并行的优势将会丧失
- 多重削减的操作将难以实现

##### Multi-tier Pattern 多级模式

上下文：在分布式部署中，通常需要将系统的基础结构分布到不同的子集中。 这可能是出于运营或业务原因。

问题：我们如何将系统拆分为由某些通信介质连接的许多计算独立的执行结构？

解决方案：许多系统的执行结构被组织为一系列组件的逻辑分组。每个分组称为级。

关系：xx是xx的一部分，xx与xx沟通，xx分配给xx

约束：一个软件组件恰好属于一个层

弱点：大量的前期成本和复杂性

#### 架构模式和战术的关系

模式包括战术，也可以使用战术在增强模式

#### 代理模式的缺点(书上特地提到)

- 可用性。服务器，代理甚至客户端的活动都需要进行监控，并且必须提供修复机制。
- 性能。客户端和服务器之间的间接级别增加了开销，因此增加了延迟。 同样，如果不需要客户端和服务器之间的直接通信，则代理是潜在的性能瓶颈。
- 可测试性。代理用于复杂的多进程和多处理器系统中。 这样的系统通常是高度动态的，请求和响应通常是异步的，这些使测试和调试此类系统极为困难。但代理模式的描述不提供测试功能，例如测试接口，状态或活动捕获和回放功能，等等。
- 安全性。因为代理模式主要在系统跨越进程和处理器边界（例如在基于Web的系统上）时使用，所以安全是一个合理的问题。 但是，所提供的代理模式不提供任何身份验证或授权客户端或服务器的方法，也不提供保护客户端与服务器之间的通信的方法。

使用什么战术来改进这一模式呢？

- 增加可用资源性能策略将导致多个代理，以帮助提高性能和可用性。
- 维护多副本策略将允许这些代理中的每个共享状态，以确保它们对客户端请求的响应相同。
- 负载平衡将确保一个代理不过载，而另一个代理处于空闲状态。
- 心跳，异常检测或ping / echo将为复制的代理提供一种通知客户端并在其中一个客户端停止服务时互相通知的方法，以作为检测故障的一种方法。

### 质量属性建模和分析

林连南曾经有道题(可以看到本文最后图片，第7题)，让大家建立一个模型来分析MVC性能。这其实是书上p252～254的内容。待续。

### 敏捷项目中的架构

主要是书p275，敏捷和体系结构的关系：从来不是二选一。虽然不能很好地共存，但二者皆不可缺少。

书上p279的An analytic perspective on up-front work vs. agility。主要使用了the constructive cost model II (COCOMO II)来对多个项目进行分析。书p280的图反映了项目整体时间(y)和架构和风险的解决时间(x)的关系(都是百分比)。Sweet point就是让项目总时间最少的前期工作时间比。可以看到，KSLOC(thousands of source lines of code)越大的项目，前期工作做得越差，项目总时间越多，越不太可能敏捷。

早熟的问题可以通过重构解决。

### 架构和要求(书Chap. 16，重点)

架构上的重要要求(architecturally significant requirement，ASR)是对架构有深远影响的要求。

获取ASR的方法：

1. Gathering ASRs from requirements documents 从需求文档中获取。
2. Gathering ASRs by interviewing stakeholders 采访涉众，这里谈到了Quality Attribute Workshop(QAW，质量属性研讨会)。QAW专注于系统级的关注，特别是软件在系统中的作用。
3. Gathering ASRs by understanding the business goals 理解业务目标。这里谈到了the Pedigreed Attribute eLicitation Method(PALM，属性激励方法)。其7个步骤为：
   1. PALM overview presentation PALM概述演示
   2. Business drivers presentation 业务驱动力介绍
   3. Architecture drivers presentation 架构驱动程序演示
   4. Business goals elicitation 业务目标启发
   5. Identification of potential quality attributes from business goals 从业务目标中识别潜在的质量属性
   6. Assignment of pedigree to existing quality attribute drivers 将谱系分配给现有的质量属性动因
   7. Exercise conclusion 结论
4. Capturing ASRs in a utility tree 在效用树中捕获。

ASR必须具有以下特征：

- 对架构的深刻影响。
- 较高的业务或使命价值。

书p307给了个很全的效用树，写成了表格的形式，不过是把根节点隐去而已。

### 设计架构

主要是书p316的质量驱动设计方法(The attrebute-driven design method, ADD)，它可以做到：

- 选择系统的一部分进行设计
- 整理该部分在架构上的所有重要要求
- 创建并测试该部分的设计

在设计架构之前，架构的功能、质量和约束都应该知道，但这种事情显然是不可能做到的。ADD可以在一组在结构上很重要的要求是已知的的情况下使用。ADD的输出是一组架构视图的草图，这些视图将共同标识架构元素及其关系或交互的集合。

ADD的步骤为：

1. 选择要设计的系统元素。
2. 确定所选元素的ASR。
3. 为所选元素生成设计解决方案。
4. 盘存剩余需求，并为下一次迭代选择输入。
5. 重复以上四步直到所有ASR都被满足。

### 架构的实现和测试

书p364。架构侵蚀：例如有时实现者采用了一些实现，使得架构模块化程度降低。

帮助保持代码和架构一致的四种技术：

1. Embedding the design in the code 将设计嵌入代码中
2. Frameworks 框架
3. Code templates 代码模板
4. Keeping code and architecture consistent 保持代码和架构一致

书p370。各种测试类别：

- 单元测试：测试系统一部分
- 整合测试：测试各个单元一起工作的情况
- 系统测试：直接测试整个系统
- 验收测试：看是否达到用户需求
- 黑盒测试：不依赖系统内部设计、结构或实现信息来测试
- 白盒测试：跟上面反过来
- 灰盒测试：使用部分有用的设计、结构或实现信息来测试
- 基于风险的测试。

架构师在测试中的角色：

1. 测试计划
2. 测试开发
3. 测试解释
4. 测试工具构建

### 架构评估

架构评估的三种形式及其特点

1. Evaluation by the designer within the design process 设计者在设计过程中的评估
2. Evaluation by peers within the design process 在设计过程中由同行评估
3. Analysis by outsiders once the architecture has been designed 设计架构后由外部人员进行分析

#### The architecture tradeoff analysis method(ATAM)

ATAM的设计使得评估者不需要熟悉架构或其业务目标，系统不需要构建，并且可能有大量的涉众。

ATAM的参与者：

1. The evaluation team
2. Project decision makers
3. Architecture stakeholders

ATAM的步骤：

1. Present the ATAM 展示ATAM
2. Present business drivers 展示业务驱动力
3. Present the architecture 展示架构
4. Identify architectural approaches 确定架构方法
5. Generate utility tree 生成效用树
6. Analyze architectural approaches 分析架构方法
7. Brainstorm and prioritize scenarios 集思广益并确定方案的优先级
8. Analyze architectural approaches 分析架构方法
9. Present results 呈现结果





### 林博选的课后题(这学期和以前的)

p77 第一题

问题：What’s the relationship between a use case and a quality attribute scenario? If you wanted to add quality attribute information to a use case, how would you do it ?

翻译：用例和质量属性场景之间有什么关系？ 如果要将质量属性信息添加到用例中，您将如何进行？

解答：用例和质量属性场景是同一件事情的不同表示方法。在软件工程中，用例描述用户想要实现的功能，而质量属性方案包含功能在设计的应用程序中如何运行的描述。功能和质量属性是正交的，如果要将质量属性信息添加到用例，那就添加新的用例。质量属性场景在质量属性需求规范中的作用与用例在功能需求规范中所扮演的角色相同。每个用例提供了一个或多个场景，该场景揭示了系统是如何同最终用户或其他系统交互的，从而获得一个明确的业务目标。用例要避免技术术语，取而代之的是最终用户或者领域。

p78 第六题

问题：Consider the choice between synchronous and asynchronous communication (a choice in the coordination mechanism category). What quality attribute requirements might lead you choose one over the other.

翻译：请考虑在同步和异步通信机制之间进行选择（在协调机制下选择）。是什么质量属性要求导致你选择这一个而不是另一个？

解答：同步通信效率更高，更加适合对速度要求高的传输，在有性能这一质量属性要求下会选择同步通信。异步通信是点对点的，对时序要求低，在要求易用性这一质量属性时会选择异步通信。同步是当用户发送一个请求，需要等待返回后才能发送下一个请求，用户有等待时间。异步是增加了消息队列，将用户请求放入队列中等待返回，用户无需等待。

p78 第七题

问题：Consider the choice between stateful and stateless communication (a choice in the coordination mechanism category). What quality attribute requirement might lead you to choose one over the other ?

翻译：请考虑在有状态和无状态通信机制之间进行选择(在协调机制下选择)。是什么质量属性要求导致你选择某一个。

解答：有状态通信无需额外的调用，具有低延迟的优点，在性能和可用性的质量属性要求下选择；无状态通信有防止数据丢失的优点，在安全性的质量属性要求下选择。

p100 第一题

问题：Write a set of concrete scenarios for availability using each of the possible responses in the general scenario.

翻译：可用性的通用场景下有多个可能的响应，为每个响应写一个通用场景。

解答：下面有

p158 第一题

问题：Write a set of concrete scenarios for security for an automatic teller machine. How would you modify your design for the automatic teller machine to satisfy these scenarios? 

翻译：请描述一组ATM（自动柜员机）的具体安全场景，并回答你将如何修改ATM的设计以满足这些安全需求？

解答：下面有



### 林博给的题(czq版)

![](/home/jhseng/桌面/software_architecture_summary/Photos/1.jpg)

![](/home/jhseng/桌面/software_architecture_summary/Photos/2.jpg)

![](/home/jhseng/桌面/software_architecture_summary/Photos/3.jpg)

![](/home/jhseng/桌面/software_architecture_summary/Photos/4.jpg)

![](/home/jhseng/桌面/software_architecture_summary/Photos/5.jpg)

![](/home/jhseng/桌面/software_architecture_summary/Photos/6.jpg)

![](/home/jhseng/桌面/software_architecture_summary/Photos/7.jpg)



Date：12/03/2019

Author：JHSeng