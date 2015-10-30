#include "widget.h"
#include "ui_widget.h"

#include "addhadith.h"

#include <QDebug>
#include <QTimer>
#include <QTextStream>
#include <QDir>
#include <QMessageBox>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{

    ui->setupUi(this);

    try {

        // SETUP THE RECORD FILE
        QString dirQString = "/.kotab/files/";
        QDir dir(QDir::homePath() + dirQString);
        if(!dir.exists()) dir.mkpath(QDir::homePath() + dirQString);
        recordFile = new QFile(dir.absolutePath() + "/" + "record.txt");
        qDebug() << dir.absolutePath();

        // SETUP THE AHADITH DATABASE
        QString databasePath = dir.absolutePath()+ "/" + "ahadith.db";
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(databasePath);

        if(!QFile(databasePath).exists()) {
            db.open();
            QSqlQuery query;
            query.exec("CREATE TABLE hadith"
                      "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "rawi VARCHAR(50), "
                      "hadith TEXT)");
            db.close();
        }
    } catch(...) {
        qDebug() << "Exception";
        return;
    }


    readDataFromDatabase();

    pinkHtml = "<font color=\"DeepPink\">";
    greenHtml = "<font color=\"Lime\">";
    blackHtml = "<font color=\"Black\">";
    endHtml = "</font>";

    // DISBALE THE HADITH TEXT EDIT
    ui->hadithTextEdit->setReadOnly(true);


    // CREATE THE COUNTDOWN
    timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(timeOutSlot()));
    // SET A TEXT CHANGE LISTNER
    connect(ui->wordLineEdit, SIGNAL(textChanged(QString)), this, SLOT(wordChanged(QString)));
    // SET A BUTTON CLICK LISTNER
    connect(ui->trainAgainButton, SIGNAL(clicked(bool)), this, SLOT(trainAgainButtonClicked(bool)));
    connect(ui->addHadithButton, SIGNAL(clicked(bool)), this, SLOT(openAddHadith(bool)));

    // CHECK IF THE LIST IS EMPTY
    // SHOW A MESSAGE TO ADD A HADITH
    if(ahadithList.size() == 0) {

        Hadith hadith;
        hadith.setArawi("البخاري");
        hadith.setHadith("اللهم أنت ربي لا إله إلا أنت خلقتني وأنا عبدك وأنا على عهدك ووعدك ما استطعت أعوذ بك من شر ما صنعت أبوء لك بنعمتك علي وأبوء لك بذنبي فاغفر لي فإنه لا يغفر الذنوب إلا أنت.");
        addHadith(hadith);
        hadith.setArawi("مسلم");
        hadith.setHadith("اللهم إني أعوذ بك من العجز والكسل والجبن والبخل والهرم وعذاب القبر. اللهم آت نفسي تقواها وزكها أنت خير من زكاها أنت وليها ومولاها. اللهم إني أعوذ بك من علم لا ينفع ومن قلب لا يخشع ومن نفس لا تشبع ومن دعوة لا يستجاب لها.");
        addHadith(hadith);
        hadith.setArawi("البخاري");
        hadith.setHadith("اللهم لك الحمد أنت قيم السموات والأرض ومن فيهن، ولك الحمد لك ملك السموات والأرض ومن فيهن، ولك الحمد أنت نور السموات والأرض ومن فيهن، ولك الحمد أنت ملك السموات والأرض. ولك الحمد أنت الحق، ووعدك الحق، ولقاؤك حق، وقولك حق، والجنة حق، والنار حق، والنبيون حق، ومحمد - صلى الله عليه وسلم - حق، والساعة حق. اللهم لك أسلمت، وبك آمنت، وعليك توكلت، وإليك أنبت، وبك خاصمت، وإليك حاكمت؛ فاغفر لي ما قدمت وما أخرت، وما أسررت، وما أعلنت. أنت المقدم وأنت المؤخر لا إله إلا أنت.");
        addHadith(hadith);
        hadith.setArawi("الترمذي");
        hadith.setHadith("اللهم رب السموات السبع ورب العرش العظيم ربنا ورب كل شيء منزل التوراة والإنجيل والقرآن فالق الحب والنوى أعوذ بك من شر كل شيء أنت آخذ بناصيته أنت الأول فليس قبلك شيء وأنت الظاهر فليس فوقك شيء وأنت الباطن فليس دونك شيء اقض عني الدين وأغنني من الفقر.");
        addHadith(hadith);
        hadith.setArawi("الترمذي");
        hadith.setHadith("اللهم إني أعوذ بك من قلب لا يخشع ومن دعاء لا يسمع ومن نفس لا تشبع ومن علم لا ينفع وأعود بك من هؤلاء الأربع.");
        addHadith(hadith);
        hadith.setArawi("متفق عليه");
        hadith.setHadith("اللهم لك الحمد أنت نور السماوات والأرض ومن فيهن ولك الحمد، أنت قيوم السماوات والأرض ومن فيهن ولك الحمد، أنت الحق، ووعدك الحق، ولقاؤك حق، والجنة حق، والنار حق، والنبيون حق، والساعة حق، ومحمد حق، اللهم لك أسلمت، وبك آمنت، وعليك توكلت، وإليك أنبت، وبك خاصمت، وإليك حاكمت، فاغفر لى ما قدمت وما أخرت، وما أسررت وما أعلنت، أنت إلهى لا إله إلا أنت.");
        addHadith(hadith);
        hadith.setArawi("مسلم");
        hadith.setHadith("اللهم صل على محمد، وعلى آل محمد، كما صليت على إبراهيم، وعلى آل إبراهيم، إنك حميد مجيد، وبارك على محمد، وعلى آل محمد، كما باركت على إبراهيم، وعلى آل إبراهيم، في العالمين إنك حميد مجيد");
        addHadith(hadith);
        hadith.setArawi("البيهقي");
        hadith.setHadith("اللهم أحيني على سنة نبيك وتوفني على ملته، وأعذني من مضلات الفتن.");
        addHadith(hadith);
        hadith.setArawi("الترمذي");
        hadith.setHadith("أستغفر الله العظيم الذي لا إله إلا هو، الحي القيوم، وأتوب إليه.");
        addHadith(hadith);
        hadith.setArawi("مسلم");
        hadith.setHadith("رب اغفر لي خطيئتي يوم الدين.");
        addHadith(hadith);
        hadith.setArawi("أحمد بن حنبل");
        hadith.setHadith("اللهم اهدني فيمن هديت، وعافني فيمن عافيت، وتولني فيمن توليت، وبارك لي فيما أعطيت، وقني شرما قضيت، إنه لا يذل من واليت، تباركت ربنا وتعاليت.");
        addHadith(hadith);
        hadith.setArawi("البخاري");
        hadith.setHadith("اللهم اغفر لي، اللهم اجعلني يوم القيامة فوق كثير من خلقك من الناس، اللهم اغفر لي ذنبي، وأدخلني يوم القيامة مدخلا كريما.");
        addHadith(hadith);
        hadith.setArawi("مسلم");
        hadith.setHadith("اللهم إني أسألك شهادة في سبيلك.");
        addHadith(hadith);
        hadith.setArawi("مسلم");
        hadith.setHadith("اللهم إني أعوذ بك من عذاب النار، وأعوذ بك من عذاب القبر، وأعوذ بك من الفتن ما ظهر منها وما بطن، وأعوذ بك من فتنة الدجال.");
        addHadith(hadith);
        hadith.setArawi("البخاري");
        hadith.setHadith("اللهم إني أعوذ بك من الهم والحزن، والعجز والكسل، والبخل والجبن ، وضلع الدين، وغلبة الرجال.");
        addHadith(hadith);
        hadith.setArawi("إبن ماجه");
        hadith.setHadith("اللهم إني أسألك المعافاة في الدنيا والآخرة.");
        addHadith(hadith);
        hadith.setArawi("أحمد بن حنبل");
        hadith.setHadith("اللهم لا تخزني يوم القيامة.");
        addHadith(hadith);
        hadith.setArawi("أبو داود");
        hadith.setHadith("اللهم إني أعوذ بك من صلاة لا تنفع.");
        addHadith(hadith);
        hadith.setArawi("الألباني");
        hadith.setHadith("اللهم إني أعوذ بك من جار السوء، ومن زوج تشيبني قبل المشيب، ومن ولد يكون علي ربا، ومن مال يكون علي عذابا، ومن خليل ماكر عينه تراني، وقلبه يرعاني؛ إن رأى حسنة دفنها، وإذا رأى سيئة أذاعها.");
        addHadith(hadith);
        hadith.setArawi("الألباني");
        hadith.setHadith("اللهم جدد الإيمان في قلبي.");
        addHadith(hadith);
        hadith.setArawi("البخاري");
        hadith.setHadith("اللهم إني أسألك الفردوس أعلى الجنة.");
        addHadith(hadith);
        hadith.setArawi("الألباني");
        hadith.setHadith("اللهم إني أسألك الثبات في الأمر، والعزيمة على الرشد، وأسألك موجبات رحمتك، وعزائم مغفرتك، وأسألك شكر نعمتك، وحسن عبادتك، وأسألك قلبا سليما، ولسانا صادقا، وأسألك من خير ما تعلم، وأعوذ بك من شر ما تعلم، وأستغفرك لما تعلم، إنك أنت علام الغيوب.");
        addHadith(hadith);
        hadith.setArawi("البخاري");
        hadith.setHadith("اللهم ثبتني واجعلني هاديا مهديا.");
        addHadith(hadith);
        hadith.setArawi("الترميذي");
        hadith.setHadith("اللهم زدنا ولا تنقصنا، وأكرمنا ولا تهنا، وأعطنا ولا تحرمنا، وآثرنا ولا تؤثر علينا، وأرضنا وارض عنا.");
        addHadith(hadith);
        hadith.setArawi("مسلم");
        hadith.setHadith("اللهم اغفر لي، وارحمني، واهدني، وعافني، وارزقني.");
        addHadith(hadith);
        hadith.setArawi("الألباني");
        hadith.setHadith("اللهم لك الحمد كله، اللهم لا قابض لما بسطت، ولا باسط لما قبضت، ولا هادي لمن أضللت، ولا مضل لمن هديت، ولا معطي لما منعت، ولا مانع لما أعطيت، ولا مقرب لما باعدت، ولا مباعد لما قربت، اللهم ابسط علينا من بركاتك، ورحمتك، وفضلك، ورزقك، اللهم إني أسألك النعيم المقيم الذي لا يحول ولا يزول، اللهم إني أسألك النعيم يوم العيلة، والأمن يوم الخوف، اللهم إني عائذ بك من شر ما أعطيتنا وشر ما منعتنا، اللهم حبب إلينا الإيمان وزينه في قلوبنا، وكره إلينا الكفر والفسوق والعصيان، واجعلنا من الراشدين، اللهم توفنا مسلمين، وأحينا مسلمين، وألحقنا بالصالحين غير خزايا ولا مفتونين، اللهم قاتل الكفرة الذين يكذبون رسلك، ويصدون عن سبيلك، واجعل عليهم رجزك وعذابك، اللهم قاتل كفرة الذين أوتوا الكتاب، إله الحق.");
        addHadith(hadith);
        hadith.setArawi("الترميذي");
        hadith.setHadith("اللهم متعني بسمعي، وبصري، واجعلهما الوارث مني، وانصرني على من يظلمني، وخذ منه بثأري.");
        addHadith(hadith);
        hadith.setArawi("النسائي");
        hadith.setHadith("اللهم اغفر لي، واهدني، وارزقني، وعافني، أعوذ بالله من ضيق المقام يوم القيامة.");
        addHadith(hadith);
        hadith.setArawi("النسائي");
        hadith.setHadith("اللهم إني أعوذ بك من غلبة الدين، وغلبة العدو، وشماتة الأعداء.");
        addHadith(hadith);
        hadith.setArawi("أحمد بن حنبل");
        hadith.setHadith("اللهم حاسبني حسابا يسيرا.");
        addHadith(hadith);
        hadith.setArawi("إبن ماجه");
        hadith.setHadith("اللهم إني أسألك علما نافعا،وأعوذ بك من علم لا ينفع.");
        addHadith(hadith);
        hadith.setArawi("الترمذي");
        hadith.setHadith("اللهم ألهمني رشدي، وأعذني من شر نفسي.");
        addHadith(hadith);
        hadith.setArawi("النسائي");
        hadith.setHadith("اللهم رب جبرائيل، وميكائيل، ورب إسرافيل، أعوذ بك من حر النار، ومن عذاب القبر.");
        addHadith(hadith);
        hadith.setArawi("النسائي");
        hadith.setHadith("اللهم إني أعوذ بك من البخل، والجبن، وسوء العمر، وفتنة الصدر، وعذاب القبر.");
        addHadith(hadith);
        hadith.setArawi("مسلم");
        hadith.setHadith("اللهم طهرني من الذنوب والخطايا، اللهم نقني منها كما ينقى الثوب الأبيض من الدنس، اللهم طهرني بالثلج والبرد والماء البارد.");
        addHadith(hadith);
        hadith.setArawi("الترميذي");
        hadith.setHadith("اللهم ارزقني حبك، وحب من ينفعني حبه عندك، اللهم ما رزقتني مما أحب فاجعله قوة لي فيما تحب، اللهم ما زويت عني مما أحب فاجعله فراغا لي فيما تحب.");
        addHadith(hadith);
        hadith.setArawi("النسائي");
        hadith.setHadith("للهم بعلمك الغيب، وقدرتك على الخلق، أحيني ما علمت الحياة خيرا لي، وتوفني إذا علمت الوفاة خيرا لي، اللهم إني أسألك خشيتك في الغيب والشهادة، وأسألك كلمة الحق في الرضا والغضب، وأسألك القصد في الغنى والفقر، وأسألك نعيما لا ينفد، وأسألك قرة عين لا تنقطع، وأسألك الرضا بعد القضاء، وأسألك برد العيش بعد الموت، وأسألك لذة النظر إلى وجهك، والشوق إلى لقائك، في غير ضراء مضرة، ولا فتنة مضلة، اللهم زينا بزينة الإيمان، واجعلنا هداة مهتدين.");
        addHadith(hadith);
        hadith.setArawi("إبن ماجه");
        hadith.setHadith("رب اغفر لي، وتب علي، إنك أنت التواب الغفور.");
        addHadith(hadith);
        hadith.setArawi("الترميذي");
        hadith.setHadith("اللهم إني أسألك بأني أشهد أنك أنت الله لا إله إلا أنت، الأحد، الصمد، الذي لم يلد، ولم يولد، ولم يكن له كفوا أحد.");
        addHadith(hadith);
        hadith.setArawi("النسائي");
        hadith.setHadith("اللهم إني أسألك بأن لك الحمد، لا إله إلا أنت وحدك لا شريك لك المنان بديع السموات والأرض، يا ذا الجلال والإكرام، يا حي يا قيوم، إني أسألك [الجنة وأعوذ بك من النار.");
        addHadith(hadith);
        hadith.setArawi("النسائي");
        hadith.setHadith("اللهم إني أسألك يا الله بأنك الواحد الأحد، الصمد، الذي لم يلد ولم يولد، ولم يكن له كفوا أحد، أن تغفر لي ذنوبي، إنك أنت الغفور الرحيم.");
        addHadith(hadith);
        hadith.setArawi("إبن ماجه");
        hadith.setHadith("اللهم إني أسألك علما نافعا، ورزقا طيبا، وعملا متقبلا.");
        addHadith(hadith);
        hadith.setArawi("إبن ماجه");
        hadith.setHadith("اللهم انفعني بما علمتني، وعلمني ما ينفعني، وزدني علما.");
        addHadith(hadith);
        hadith.setArawi("البخاري");
        hadith.setHadith("اللهم فقهني في الدين.");
        addHadith(hadith);
        hadith.setArawi("الترميذي");
        hadith.setHadith("اللهم إني أسألك الجنة وأستجير بك من النار.");
        addHadith(hadith);
        hadith.setArawi("النسائي");
        hadith.setHadith("اللهم إني أعوذ بك من الجوع؛ فإنه بئس الضجيع، وأعوذ بك من الخيانة؛ فإنها بئست البطانة.");
        addHadith(hadith);
        hadith.setArawi("النسائي");
        hadith.setHadith("اللهم إني أعوذ بك من الهرم، والتردي، والهدم، والغم، والغرق، والحرق، وأعوذ بك أن يتخبطني الشيطان عند الموت، وأعوذ بك أن أموت في سبيلك مدبرا، وأعوذ بك أن أموت لديغا.");
        addHadith(hadith);
        hadith.setArawi("متفق عليه");
        hadith.setHadith("اللهم لك أسلمت، وبك آمنت، وعليك توكلت، وإليك أنبت، وبك خاصمت. اللهم إني أعوذ بعزتك لا إله إلا أنت أن تضلني، أنت الحي الذي لا يموت، والجن والإنس يموتون.");
        addHadith(hadith);
        hadith.setArawi("متفق عليه");
        hadith.setHadith("اللهم إني ظلمت نفسي ظلما كثيرا، ولا يغفر الذنوب إلا أنت. فاغفر لي مغفرة من عندك، وارحمني إنك أنت الغفور الرحيم.");
        addHadith(hadith);
        hadith.setArawi("مسلم");
        hadith.setHadith("اللهم اغفر لي خطيئتي، وجهلي، وإسرافي في أمري، وما أنت أعلم به مني، اللهم اغفر لي هزلي، وجدي، وخطئي، وعمدي، وكل ذلك عندي اللهم اغفر لي ما قدمت وما أخرت وما أسررت وما أعلنت وما أنت أعلم به مني أنت المقدم وأنت المؤخر وأنت على كل شيء قدير.");
        addHadith(hadith);
        hadith.setArawi("البخاري");
        hadith.setHadith("اللهم إني أعوذ بك من الجبن، وأعوذ بك من البخل، وأعوذ بك من أن أرد إلى أرذل العمر، وأعوذ بك من فتنة الدنيا وعذاب القبر.");
        addHadith(hadith);
        hadith.setArawi("الترميذي");
        hadith.setHadith("اللهم اقسم لنا من خشيتك ما تحول به بيننا وبين معاصيك، ومن طاعتك ما تبلغنا به جنتك، ومن اليقين ما تهون به علينا مصائب الدنيا، اللهم متعنا بأسماعنا، وأبصارنا، وقواتنا ما أحييتنا، واجعله الوارث منا، واجعل ثأرنا على من ظلمنا، وانصرنا على من عادانا، ولا تجعل مصيبتنا في ديننا، ولا تجعل الدنيا أكبر همنا، ولا مبلغ علمنا، ولا تسلط علينا من لا يرحمنا.");
        addHadith(hadith);
        hadith.setArawi("إبن ماجه");
        hadith.setHadith("اللهم إني أسألك من الخير كله عاجله وآجله، ما علمت منه وما لم أعلم، وأعوذ بك من الشر كله عاجله وآجله، ما علمت منه وما لم أعلم. اللهم إني أسألك من خير ما سألك عبدك ونبيك، وأعوذ بك من شر ما استعاذ بك منه عبدك ونبيك. اللهم إني أسألك الجنة، وما قرب إليها من قول أو عمل، وأعوذ بك من النار وما قرب إليها من قول أو عمل، وأسألك أن تجعل كل قضاء قضيته لي خيرا.");
        addHadith(hadith);
        hadith.setArawi("الترميذي");
        hadith.setHadith("اللهم إني أسألك فعل الخيرات، وترك المنكرات، وحب المساكين، وأن تغفر لي، وترحمني، وإذا أردت فتنة قوم فتوفني غير مفتون، وأسألك حبك، وحب من يحبك، وحب عمل يقربني إلى حبك.");
        addHadith(hadith);
        hadith.setArawi("الترمذي");
        hadith.setHadith("اللهم إني أعوذ بك من منكرات الأخلاق، والأعمال، والأهواء.");
        addHadith(hadith);
        hadith.setArawi("النسائي");
        hadith.setHadith("اللهم إني أعوذ بك من البرص، والجنون، والجذام، ومن سيئ الأسقام.");
        addHadith(hadith);
        hadith.setArawi("الترميذي");
        hadith.setHadith("اللهم إني أعوذ بك من شر سمعي، ومن شر بصري، ومن شر لساني، ومن شر قلبي، ومن شر منيي.");
        addHadith(hadith);
        hadith.setArawi("الترميذي");
        hadith.setHadith("اللهم إنا نسألك من خير ما سألك منه نبيك محمد صلى الله عليه وسلم، ونعوذ بك من شر ما استعاذ منه نبيك محمد صلى الله عليه وسلم، وأنت المستعان، وعليك البلاغ، ولا حول ولا قوة إلا بالله.");
        addHadith(hadith);
        hadith.setArawi("أبو داود");
        hadith.setHadith("رب أعني ولا تعن علي، وانصرني ولا تنصر علي، وامكر لي ولا تمكر علي، واهدني ويسر الهدى إلي، وانصرني على من بغى علي، رب اجعلني لك شكارا، لك ذكارا، لك رهابا، لك مطواعا، إليك مخبتا أواها منيبا، رب تقبل توبتي، واغسل حوبتي، وأجب دعوتي، وثبت حجتي، واهد قلبي، وسدد لساني، واسلل سخيمة قلبي.");
        addHadith(hadith);
        hadith.setArawi("أحمد بن حنبل");
        hadith.setHadith("اللهم أحسن عاقبتنا في الأمور كلها، وأجرنا من خزي الدنيا وعذاب الآخرة.");
        addHadith(hadith);
        hadith.setArawi("إبن ماجه");
        hadith.setHadith("اللهم إني أسألك اليقين والعفو والعافية في الدنيا والآخرة.");
        addHadith(hadith);
        hadith.setArawi("مسلم");
        hadith.setHadith("اللهم مصرف القلوب صرف قلوبنا على طاعتك.");
        addHadith(hadith);
        hadith.setArawi("الترميذي");
        hadith.setHadith("لا إله إلا أنت سبحانك إني كنت من الظالمين.");
        addHadith(hadith);
        hadith.setArawi("أبو داود");
        hadith.setHadith("اللهم رحمتك أرجو فلا تكلني إلى نفسي طرفة عين، وأصلح لي شأني كله لا إله إلا أنت.");
        addHadith(hadith);
        hadith.setArawi("البخاري");
        hadith.setHadith("لا إله إلا الله العظيم الحليم، لا إله إلا الله رب العرش العظيم، لا إله إلا الله رب السموات، ورب الأرض، ورب العرش الكريم.");
        addHadith(hadith);
        hadith.setArawi("مسلم");
        hadith.setHadith("اللهم أكثر مالي، وولدي، وبارك لي فيما أعطيتني.");
        addHadith(hadith);
        hadith.setArawi("مسلم");
        hadith.setHadith("اللهم إني أعوذ بك من شر ما عملت، ومن شر ما لم أعمل.");
        addHadith(hadith);
        hadith.setArawi("مسلم");
        hadith.setHadith("اللهم إني أعوذ بك من زوال نعمتك، وتحول عافيتك، وفجاءة نقمتك، وجميع سخطك.");
        addHadith(hadith);
        hadith.setArawi("مسلم");
        hadith.setHadith("اللهم اهدني وسددني، اللهم إني أسألك الهدى والسداد.");
        addHadith(hadith);
        hadith.setArawi("مسلم");
        hadith.setHadith("اللهم إني أعوذ بك من العجز، والكسل، والجبن، والبخل، والهرم، وعذاب القبر، اللهم آت نفسي تقواها، وزكها أنت خير من زكاها، أنت وليها ومولاها، اللهم إني أعوذ بك من علم لا ينفع، ومن قلب لا يخشع، ومن نفس لا تشبع، ومن دعوة لا يستجاب لها.");
        addHadith(hadith);
        hadith.setArawi("مسلم");
        hadith.setHadith("اللهم إني أسألك الهدى، والتقى، والعفاف، والغنى.");
        addHadith(hadith);
        hadith.setArawi("مسلم");
        hadith.setHadith("اللهم أصلح لي ديني الذي هو عصمة أمري، وأصلح لي دنياي التي فيها معاشي، وأصلح لي آخرتي التي فيها معادي، واجعل الحياة زيادة لي في كل خير، واجعل الموت راحة لي من كل شر.");
        addHadith(hadith);
        hadith.setArawi("البخاري");
        hadith.setHadith("اللهم إني أعوذ بك من جهد البلاء، ودرك الشقاء، وسوء القضاء، وشماتة الأعداء.");
        addHadith(hadith);
        hadith.setArawi("البخاري");
        hadith.setHadith("اللهم إني أعوذ بك من العجز، والكسل، والجبن، والهرم، والبخل، وأعوذ بك من عذاب القبر، ومن فتنة المحيا والممات.");
        addHadith(hadith);
        hadith.setArawi("مسلم");
        hadith.setHadith("اللهم إني أعوذ بك من فتنة النار وعذاب النار، وفتنة القبر، وعذاب القبر، وشر فتنة الغنى، وشر فتنة الفقر، اللهم إني أعوذ بك من شر فتنة المسيح الدجال، اللهم اغسل قلبي بماء الثلج والبرد، ونق قلبي من الخطايا كما نقيت الثوب الأبيض من الدنس، وباعد بيني وبين خطاياي كما باعدت بين المشرق والمغرب. اللهم إني أعوذ بك من الكسل والمأثم والمغرم.");
        addHadith(hadith);
        hadith.setArawi("مسلم");
        hadith.setHadith("اللهم آتنا في الدنيا حسنة،وفي الآخرة حسنة وقنا عذاب النار.");
        addHadith(hadith);
        hadith.setArawi("مقتبس من سورة التغابن، الآية: 16.");
        hadith.setHadith("اللهم قني شح نفسي واجعلني من المفلحين.");
        addHadith(hadith);
        hadith.setArawi("مقتبس من سورة الحجرات، الآية: 7");
        hadith.setHadith("اللهم حبب إلينا الإيمان وزينه في قلوبنا، وكره إلينا الكفر والفسوق والعصيان، واجعلنا من الراشدين.");
        addHadith(hadith);
        hadith.setArawi("مقتبس من سورة إبراهيم، الآية: 27.");
        hadith.setHadith("اللهم ثبتني بالقول الثابت في الحياة الدنيا وفي الآخرة.");
        addHadith(hadith);
        hadith.setArawi("مقتبس من سورة البقرة، الآية:269");
        hadith.setHadith("اللهم آتني الحكمة التي من أوتيها فقد أوتي خيرا كثيرا.");
        addHadith(hadith);
        hadith.setArawi("مقتبس من سورة البقرة، الآية: 213");
        hadith.setHadith("اللهم اهدني لما اختلف فيه من الحق بإذنك، إنك تهدي من تشاء إلى صراط مستقيم.");
        addHadith(hadith);
        hadith.setArawi("مقتبس من سورة البقرة، الآية: 213");
        hadith.setHadith("اللهم إني أعوذ بك من الكسل والهرم والمأثم والمغرم ومن فتنة القبر وعذاب القبر ومن فتنة النار وعذاب النار ومن شر فتنة الغنى وأعوذ بك من فتنة الفقر وأعوذ بك من فتنة المسيح الدجال. اللهم اغسل عني خطاياي بماء الثلج والبرد ونق قلبي من الخطايا كما نقيت الثوب الأبيض من الدنس وباعد بيني وبين خطاياي كما باعدت بين المشرق والمغرب.");
        addHadith(hadith);

/*81 حديث إلى الأن 
إلى أن يتم إنهاء 100 حديث
السلام عليكم ورحمة الله
*/


        /*
        QMessageBox message(QMessageBox::Information, "إضافة حديث",
                                QObject::tr("قاعدة بيانات الأحاديث فارغة، قم من فضلك بإضافة حديث."),
                                QMessageBox::NoButton ,
                                0);

        message.exec();
        ui->wordLineEdit->setEnabled(false);
        ui->arawiLabel->setVisible(false);
        ui->trainAgainButton->setEnabled(false);
        ui->timeLabel->setVisible(false);
        ui->speedLabel->setVisible(false);

        return;
        */
    }

    setup();

}

void Widget::setup() {
    startTheCounter = false;
    isDone = false;

    record = readRecordFromFile().toInt();
    QString result;
    result.append("رقمك القياسي هو : ");
    result.append(QString::number(record));
    ui->recordLabel->setText(result);

    // GENERATE A RANDOM --Widget::~Widget()
    // MIN = 0 AND MAX = AHADITH LIST SIZE - 1
    // GET A RANDOM HADITH
    currentHadith = ahadithList[generateRandom(0, ahadithList.size())];

    // SET ARRAWI IN THE LABEL
    ui->arawiLabel->setText(ui->arawiLabel->text() + " " + currentHadith.getArawi());

    // HIDE THE RESULT FOR NOW
    ui->resultLabel->setVisible(false);
    ui->trainAgainButton->setVisible(false);

    // EMPTY THE WORD LINE EDIT
    ui->wordLineEdit->setText("");
    ui->wordLineEdit->setEnabled(true);

    // CALCULATE THE WORDS IN THE HADITH
    // CREATE A METHOD IN THE HADTH CLASS
    // CALCULATE THE TIME AND PUT IT IN THE COUNT DOWN LABEL
    // CREATE A METHOD IN THE HADITH CLASS
    currentSec = currentHadith.calculateTimeForTheHadith();
    totalTime = currentSec;
    currentSpeed = 0;
    ui->speedLabel->setText(QString::number(currentSpeed) + QString(" دقيقة في الثانية").toUtf8());
    ui->timeLabel->setText(QString::number(currentSec) + QString(" ثانية").toUtf8());

    // SETUP THE CURRENT HADITH TREATED TEXT
    currentPosition = 0;
    setupText(currentPosition);
}

int Widget::generateRandom(const int &min, const int &max)
{
    return rand() % max + min;
}

void Widget::timeOutSlot()
{
    if(!isDone) {
        if(currentSec > 0) {
            currentSec--;
        } else {
            timer->stop();
            QString result;
            result.append("النتيجة: لم تكمل في الوقت المحدد والسرعة المبينة هي للجزء الذي أنجزته فقط");
            result.append(".");
            result.append("\n");
            result.append("كانت سرعتك");
            result.append(" ");
            result.append(QString::number(currentSpeed));
            result.append(" ");
            result.append("كلمة في الدقيقة.");
            showResult(result);
        }
        currentSpeed = calculateSpeed();
        ui->speedLabel->setText(QString::number(currentSpeed) + QString(" دقيقة في الثانية").toUtf8());
        ui->timeLabel->setText(QString::number(currentSec) + QString(" ثانية").toUtf8());
    } else {
        timer->stop();
    }
}

void Widget::setupText(const int &numberOfWordsTraited) {
    if(numberOfWordsTraited <= currentHadith.getHadith().count(' ')) {
        currentHadith.setTraitedText("");
        currentHadith.setCurrentWord("");
        currentHadith.setUntraitedText("");

        // TRAITED TEXT
        for(int i=0; i<numberOfWordsTraited; i++) {
            currentHadith.setTraitedText(currentHadith.getTraitedText() + " " + currentHadith.getHadith().split(" ")[i]);
        }

        // CURRENT WORD
        currentHadith.setCurrentWord(currentHadith.getHadith().split(" ")[numberOfWordsTraited]);
        ui->hadithTextEdit->setText(currentHadith.getCurrentWord());

        // UNTRAITED TEXT
        for(int i=numberOfWordsTraited+1; i<currentHadith.getNumberOfWordsInAHadith(); i++) {
            currentHadith.setUntraitedText(currentHadith.getUntraitedText() + " " + currentHadith.getHadith().split(" ")[i]);
        }

        // SETUP THE HADITH TEXT EDIT
        ui->hadithTextEdit->setText(greenHtml + currentHadith.getTraitedText() + endHtml + " " +
                                    pinkHtml + currentHadith.getCurrentWord() + endHtml +
                                    blackHtml + currentHadith.getUntraitedText() + endHtml);
        ui->hadithTextEdit->setTextColor(QColor("black"));
        currentPosition ++;
    } else {
        isDone = true;
        if(currentSpeed > record) {
            QString result;
            result.append("مبروك لقد حطمت رقمك السابق");
            result.append(".\n");
            result.append("رقمك الجديد هو ");
            result.append(QString::number(currentSpeed));
            result.append(".");
            ui->recordLabel->setText(result);
            writeRecordToFile(currentSpeed);
        }
        QString result;
        result.append("النتيجة: ");
        result.append("\n");
        result.append("كانت سرعتك");
        result.append(" ");
        result.append(QString::number(currentSpeed));
        result.append(" ");
        result.append("كلمة في الدقيقة.");
        showResult(result);

    }
}

void Widget::wordChanged(QString word)
{
    if(!startTheCounter) {
        startTheCounter = true;
        timer->start(1000);
    }
    if(word == currentHadith.getCurrentWord() + " ") {
        ui->wordLineEdit->setText("");
        setupText(currentPosition);
    }
}

void Widget::trainAgainButtonClicked(bool)
{
    if(ahadithList.size() > 0) setup();
}

int Widget::calculateSpeed() const {
    // CURRENTTIME -> NUMBEROFWORDSTRAITED
    // 60 SEC      -> x
    return 60 * (currentHadith.getTraitedText().count(" ") + 1) / (totalTime - currentSec);
}

void Widget::showResult(const QString &result) {
    ui->wordLineEdit->setEnabled(false);
    if(!ui->resultLabel->isVisible()) ui->resultLabel->setVisible(true);
    if(!ui->trainAgainButton->isVisible()) ui->trainAgainButton->setVisible(true);
    ui->resultLabel->setText(result);
}

QString Widget::readRecordFromFile() {
    if(recordFile->exists()) {
        qDebug() << recordFile->fileName();

        recordFile->open(QIODevice::ReadOnly | QIODevice::Text);
        QString line = recordFile->readLine();
        recordFile->close();
        return line;
    }
    return "0";
}

void Widget::writeRecordToFile(const int &record) {
    recordFile->open(QIODevice::ReadWrite | QIODevice::Text);
    recordFile->flush();
    QTextStream out(recordFile);
    out << record;
    recordFile->close();
}

void Widget::readDataFromDatabase()
{
    db.open();
    QSqlQuery query;
    query.exec("SELECT * FROM hadith");
    while(query.next()) {
        QSqlRecord rec = query.record();
        Hadith hadith(rec.field(2).value().toString(), rec.field(1).value().toString());
        ahadithList.append(hadith);
    }

    db.close();
}

void Widget::openAddHadith(bool)
{
    AddHadith* addHadith = new AddHadith(this);
    addHadith->show();
    addHadith->setModal(true);
}

// TEMPORARY
void Widget::addHadith(const Hadith& hadith)
{

    // ADD HADITH TO DATABASE
    db.open();
    QSqlQuery query;
    if(query.exec(QString("INSERT INTO hadith (rawi, hadith) VALUES ('%1','%2')").arg(hadith.getArawi(), hadith.getHadith()))) {

    } else {

    }
    db.close();
}


Widget::~Widget()
{
    delete ui;
}
