#include "pch.h"
#include "res_tp_frames.h"

// this file is generated by tool: plist2blist

xx::Task<> ResTpFrames::AsyncLoad(std::string picRoot) {
	{
		auto& eg = xx::EngineBase3::Instance();
#ifdef __EMSCRIPTEN__
		auto tp = co_await eg.AsyncLoadTexturePackerFromUrl(picRoot + "pngs.blist");
#else
		auto tp = eg.LoadTexturePacker<true>(picRoot + "pngs.blist");
#endif
		xx_assert(tp);
		auto map = tp->GetMapSV();

		this->buff_996 = map["buff_996"sv];
		this->_tex_buff_996 = this->buff_996->tex;
		this->_texid_buff_996 = this->_tex_buff_996->GetValue();
		this->buff_angry = map["buff_angry"sv];
		this->_tex_buff_angry = this->buff_angry->tex;
		this->_texid_buff_angry = this->_tex_buff_angry->GetValue();
		this->buff_aniki = map["buff_aniki"sv];
		this->_tex_buff_aniki = this->buff_aniki->tex;
		this->_texid_buff_aniki = this->_tex_buff_aniki->GetValue();
		this->buff_anime_master = map["buff_anime_master"sv];
		this->_tex_buff_anime_master = this->buff_anime_master->tex;
		this->_texid_buff_anime_master = this->_tex_buff_anime_master->GetValue();
		this->buff_brother = map["buff_brother"sv];
		this->_tex_buff_brother = this->buff_brother->tex;
		this->_texid_buff_brother = this->_tex_buff_brother->GetValue();
		this->buff_chair = map["buff_chair"sv];
		this->_tex_buff_chair = this->buff_chair->tex;
		this->_texid_buff_chair = this->_tex_buff_chair->GetValue();
		this->buff_chinese_medicine = map["buff_chinese_medicine"sv];
		this->_tex_buff_chinese_medicine = this->buff_chinese_medicine->tex;
		this->_texid_buff_chinese_medicine = this->_tex_buff_chinese_medicine->GetValue();
		this->buff_crud_master = map["buff_crud_master"sv];
		this->_tex_buff_crud_master = this->buff_crud_master->tex;
		this->_texid_buff_crud_master = this->_tex_buff_crud_master->GetValue();
		this->buff_cut_artery = map["buff_cut_artery"sv];
		this->_tex_buff_cut_artery = this->buff_cut_artery->tex;
		this->_texid_buff_cut_artery = this->_tex_buff_cut_artery->GetValue();
		this->buff_elbow = map["buff_elbow"sv];
		this->_tex_buff_elbow = this->buff_elbow->tex;
		this->_texid_buff_elbow = this->_tex_buff_elbow->GetValue();
		this->buff_fengshui_master = map["buff_fengshui_master"sv];
		this->_tex_buff_fengshui_master = this->buff_fengshui_master->tex;
		this->_texid_buff_fengshui_master = this->_tex_buff_fengshui_master->GetValue();
		this->buff_fubao = map["buff_fubao"sv];
		this->_tex_buff_fubao = this->buff_fubao->tex;
		this->_texid_buff_fubao = this->_tex_buff_fubao->GetValue();
		this->buff_gatlin_god = map["buff_gatlin_god"sv];
		this->_tex_buff_gatlin_god = this->buff_gatlin_god->tex;
		this->_texid_buff_gatlin_god = this->_tex_buff_gatlin_god->GetValue();
		this->buff_glasses = map["buff_glasses"sv];
		this->_tex_buff_glasses = this->buff_glasses->tex;
		this->_texid_buff_glasses = this->_tex_buff_glasses->GetValue();
		this->buff_god = map["buff_god"sv];
		this->_tex_buff_god = this->buff_god->tex;
		this->_texid_buff_god = this->_tex_buff_god->GetValue();
		this->buff_haidilao = map["buff_haidilao"sv];
		this->_tex_buff_haidilao = this->buff_haidilao->tex;
		this->_texid_buff_haidilao = this->_tex_buff_haidilao->GetValue();
		this->buff_invest = map["buff_invest"sv];
		this->_tex_buff_invest = this->buff_invest->tex;
		this->_texid_buff_invest = this->_tex_buff_invest->GetValue();
		this->buff_leader = map["buff_leader"sv];
		this->_tex_buff_leader = this->buff_leader->tex;
		this->_texid_buff_leader = this->_tex_buff_leader->GetValue();
		this->buff_lottery_researcher = map["buff_lottery_researcher"sv];
		this->_tex_buff_lottery_researcher = this->buff_lottery_researcher->tex;
		this->_texid_buff_lottery_researcher = this->_tex_buff_lottery_researcher->GetValue();
		this->buff_meat_eater = map["buff_meat_eater"sv];
		this->_tex_buff_meat_eater = this->buff_meat_eater->tex;
		this->_texid_buff_meat_eater = this->_tex_buff_meat_eater->GetValue();
		this->buff_plastic_friendship = map["buff_plastic_friendship"sv];
		this->_tex_buff_plastic_friendship = this->buff_plastic_friendship->tex;
		this->_texid_buff_plastic_friendship = this->_tex_buff_plastic_friendship->GetValue();
		this->buff_relax = map["buff_relax"sv];
		this->_tex_buff_relax = this->buff_relax->tex;
		this->_texid_buff_relax = this->_tex_buff_relax->GetValue();
		this->buff_return_home = map["buff_return_home"sv];
		this->_tex_buff_return_home = this->buff_return_home->tex;
		this->_texid_buff_return_home = this->_tex_buff_return_home->GetValue();
		this->buff_running_shoes = map["buff_running_shoes"sv];
		this->_tex_buff_running_shoes = this->buff_running_shoes->tex;
		this->_texid_buff_running_shoes = this->_tex_buff_running_shoes->GetValue();
		this->buff_shanghai = map["buff_shanghai"sv];
		this->_tex_buff_shanghai = this->buff_shanghai->tex;
		this->_texid_buff_shanghai = this->_tex_buff_shanghai->GetValue();
		this->buff_shit_mountain = map["buff_shit_mountain"sv];
		this->_tex_buff_shit_mountain = this->buff_shit_mountain->tex;
		this->_texid_buff_shit_mountain = this->_tex_buff_shit_mountain->GetValue();
		this->buff_social = map["buff_social"sv];
		this->_tex_buff_social = this->buff_social->tex;
		this->_texid_buff_social = this->_tex_buff_social->GetValue();
		this->buff_social_fear = map["buff_social_fear"sv];
		this->_tex_buff_social_fear = this->buff_social_fear->tex;
		this->_texid_buff_social_fear = this->_tex_buff_social_fear->GetValue();
		this->buff_steel_plate = map["buff_steel_plate"sv];
		this->_tex_buff_steel_plate = this->buff_steel_plate->tex;
		this->_texid_buff_steel_plate = this->_tex_buff_steel_plate->GetValue();
		this->buff_sudo_rm_rf = map["buff_sudo_rm_rf"sv];
		this->_tex_buff_sudo_rm_rf = this->buff_sudo_rm_rf->tex;
		this->_texid_buff_sudo_rm_rf = this->_tex_buff_sudo_rm_rf->GetValue();
		this->buff_takeout = map["buff_takeout"sv];
		this->_tex_buff_takeout = this->buff_takeout->tex;
		this->_texid_buff_takeout = this->_tex_buff_takeout->GetValue();
		this->buff_techinical_debt = map["buff_techinical_debt"sv];
		this->_tex_buff_techinical_debt = this->buff_techinical_debt->tex;
		this->_texid_buff_techinical_debt = this->_tex_buff_techinical_debt->GetValue();
		this->buff_treadmill = map["buff_treadmill"sv];
		this->_tex_buff_treadmill = this->buff_treadmill->tex;
		this->_texid_buff_treadmill = this->_tex_buff_treadmill->GetValue();
		this->buff_wish_shanghai = map["buff_wish_shanghai"sv];
		this->_tex_buff_wish_shanghai = this->buff_wish_shanghai->tex;
		this->_texid_buff_wish_shanghai = this->_tex_buff_wish_shanghai->GetValue();
		this->buff_work_meal = map["buff_work_meal"sv];
		this->_tex_buff_work_meal = this->buff_work_meal->tex;
		this->_texid_buff_work_meal = this->_tex_buff_work_meal->GetValue();
		this->bullet_cash = map["bullet_cash"sv];
		this->_tex_bullet_cash = this->bullet_cash->tex;
		this->_texid_bullet_cash = this->_tex_bullet_cash->GetValue();
		this->bullet_coin5 = map["bullet_coin5"sv];
		this->_tex_bullet_coin5 = this->bullet_coin5->tex;
		this->_texid_bullet_coin5 = this->_tex_bullet_coin5->GetValue();
		this->bullet_coin10 = map["bullet_coin10"sv];
		this->_tex_bullet_coin10 = this->bullet_coin10->tex;
		this->_texid_bullet_coin10 = this->_tex_bullet_coin10->GetValue();
		this->button = map["button"sv];
		this->_tex_button = this->button->tex;
		this->_texid_button = this->_tex_button->GetValue();
		this->ef_atkdmg = map["ef_atkdmg"sv];
		this->_tex_ef_atkdmg = this->ef_atkdmg->tex;
		this->_texid_ef_atkdmg = this->_tex_ef_atkdmg->GetValue();
		this->ef_atkspd = map["ef_atkspd"sv];
		this->_tex_ef_atkspd = this->ef_atkspd->tex;
		this->_texid_ef_atkspd = this->_tex_ef_atkspd->GetValue();
		this->ef_heal = map["ef_heal"sv];
		this->_tex_ef_heal = this->ef_heal->tex;
		this->_texid_ef_heal = this->_tex_ef_heal->GetValue();
		this->ef_money = map["ef_money"sv];
		this->_tex_ef_money = this->ef_money->tex;
		this->_texid_ef_money = this->_tex_ef_money->GetValue();
		this->font_outline_32 = map["font_outline_32"sv];
		this->_tex_font_outline_32 = this->font_outline_32->tex;
		this->_texid_font_outline_32 = this->_tex_font_outline_32->GetValue();
		this->font_outline_33 = map["font_outline_33"sv];
		this->_tex_font_outline_33 = this->font_outline_33->tex;
		this->_texid_font_outline_33 = this->_tex_font_outline_33->GetValue();
		this->font_outline_34 = map["font_outline_34"sv];
		this->_tex_font_outline_34 = this->font_outline_34->tex;
		this->_texid_font_outline_34 = this->_tex_font_outline_34->GetValue();
		this->font_outline_35 = map["font_outline_35"sv];
		this->_tex_font_outline_35 = this->font_outline_35->tex;
		this->_texid_font_outline_35 = this->_tex_font_outline_35->GetValue();
		this->font_outline_36 = map["font_outline_36"sv];
		this->_tex_font_outline_36 = this->font_outline_36->tex;
		this->_texid_font_outline_36 = this->_tex_font_outline_36->GetValue();
		this->font_outline_37 = map["font_outline_37"sv];
		this->_tex_font_outline_37 = this->font_outline_37->tex;
		this->_texid_font_outline_37 = this->_tex_font_outline_37->GetValue();
		this->font_outline_38 = map["font_outline_38"sv];
		this->_tex_font_outline_38 = this->font_outline_38->tex;
		this->_texid_font_outline_38 = this->_tex_font_outline_38->GetValue();
		this->font_outline_39 = map["font_outline_39"sv];
		this->_tex_font_outline_39 = this->font_outline_39->tex;
		this->_texid_font_outline_39 = this->_tex_font_outline_39->GetValue();
		this->font_outline_40 = map["font_outline_40"sv];
		this->_tex_font_outline_40 = this->font_outline_40->tex;
		this->_texid_font_outline_40 = this->_tex_font_outline_40->GetValue();
		this->font_outline_41 = map["font_outline_41"sv];
		this->_tex_font_outline_41 = this->font_outline_41->tex;
		this->_texid_font_outline_41 = this->_tex_font_outline_41->GetValue();
		this->font_outline_42 = map["font_outline_42"sv];
		this->_tex_font_outline_42 = this->font_outline_42->tex;
		this->_texid_font_outline_42 = this->_tex_font_outline_42->GetValue();
		this->font_outline_43 = map["font_outline_43"sv];
		this->_tex_font_outline_43 = this->font_outline_43->tex;
		this->_texid_font_outline_43 = this->_tex_font_outline_43->GetValue();
		this->font_outline_44 = map["font_outline_44"sv];
		this->_tex_font_outline_44 = this->font_outline_44->tex;
		this->_texid_font_outline_44 = this->_tex_font_outline_44->GetValue();
		this->font_outline_45 = map["font_outline_45"sv];
		this->_tex_font_outline_45 = this->font_outline_45->tex;
		this->_texid_font_outline_45 = this->_tex_font_outline_45->GetValue();
		this->font_outline_46 = map["font_outline_46"sv];
		this->_tex_font_outline_46 = this->font_outline_46->tex;
		this->_texid_font_outline_46 = this->_tex_font_outline_46->GetValue();
		this->font_outline_47 = map["font_outline_47"sv];
		this->_tex_font_outline_47 = this->font_outline_47->tex;
		this->_texid_font_outline_47 = this->_tex_font_outline_47->GetValue();
		this->font_outline_48 = map["font_outline_48"sv];
		this->_tex_font_outline_48 = this->font_outline_48->tex;
		this->_texid_font_outline_48 = this->_tex_font_outline_48->GetValue();
		this->font_outline_49 = map["font_outline_49"sv];
		this->_tex_font_outline_49 = this->font_outline_49->tex;
		this->_texid_font_outline_49 = this->_tex_font_outline_49->GetValue();
		this->font_outline_50 = map["font_outline_50"sv];
		this->_tex_font_outline_50 = this->font_outline_50->tex;
		this->_texid_font_outline_50 = this->_tex_font_outline_50->GetValue();
		this->font_outline_51 = map["font_outline_51"sv];
		this->_tex_font_outline_51 = this->font_outline_51->tex;
		this->_texid_font_outline_51 = this->_tex_font_outline_51->GetValue();
		this->font_outline_52 = map["font_outline_52"sv];
		this->_tex_font_outline_52 = this->font_outline_52->tex;
		this->_texid_font_outline_52 = this->_tex_font_outline_52->GetValue();
		this->font_outline_53 = map["font_outline_53"sv];
		this->_tex_font_outline_53 = this->font_outline_53->tex;
		this->_texid_font_outline_53 = this->_tex_font_outline_53->GetValue();
		this->font_outline_54 = map["font_outline_54"sv];
		this->_tex_font_outline_54 = this->font_outline_54->tex;
		this->_texid_font_outline_54 = this->_tex_font_outline_54->GetValue();
		this->font_outline_55 = map["font_outline_55"sv];
		this->_tex_font_outline_55 = this->font_outline_55->tex;
		this->_texid_font_outline_55 = this->_tex_font_outline_55->GetValue();
		this->font_outline_56 = map["font_outline_56"sv];
		this->_tex_font_outline_56 = this->font_outline_56->tex;
		this->_texid_font_outline_56 = this->_tex_font_outline_56->GetValue();
		this->font_outline_57 = map["font_outline_57"sv];
		this->_tex_font_outline_57 = this->font_outline_57->tex;
		this->_texid_font_outline_57 = this->_tex_font_outline_57->GetValue();
		this->font_outline_58 = map["font_outline_58"sv];
		this->_tex_font_outline_58 = this->font_outline_58->tex;
		this->_texid_font_outline_58 = this->_tex_font_outline_58->GetValue();
		this->font_outline_59 = map["font_outline_59"sv];
		this->_tex_font_outline_59 = this->font_outline_59->tex;
		this->_texid_font_outline_59 = this->_tex_font_outline_59->GetValue();
		this->font_outline_60 = map["font_outline_60"sv];
		this->_tex_font_outline_60 = this->font_outline_60->tex;
		this->_texid_font_outline_60 = this->_tex_font_outline_60->GetValue();
		this->font_outline_61 = map["font_outline_61"sv];
		this->_tex_font_outline_61 = this->font_outline_61->tex;
		this->_texid_font_outline_61 = this->_tex_font_outline_61->GetValue();
		this->font_outline_62 = map["font_outline_62"sv];
		this->_tex_font_outline_62 = this->font_outline_62->tex;
		this->_texid_font_outline_62 = this->_tex_font_outline_62->GetValue();
		this->font_outline_63 = map["font_outline_63"sv];
		this->_tex_font_outline_63 = this->font_outline_63->tex;
		this->_texid_font_outline_63 = this->_tex_font_outline_63->GetValue();
		this->font_outline_64 = map["font_outline_64"sv];
		this->_tex_font_outline_64 = this->font_outline_64->tex;
		this->_texid_font_outline_64 = this->_tex_font_outline_64->GetValue();
		this->font_outline_65 = map["font_outline_65"sv];
		this->_tex_font_outline_65 = this->font_outline_65->tex;
		this->_texid_font_outline_65 = this->_tex_font_outline_65->GetValue();
		this->font_outline_66 = map["font_outline_66"sv];
		this->_tex_font_outline_66 = this->font_outline_66->tex;
		this->_texid_font_outline_66 = this->_tex_font_outline_66->GetValue();
		this->font_outline_67 = map["font_outline_67"sv];
		this->_tex_font_outline_67 = this->font_outline_67->tex;
		this->_texid_font_outline_67 = this->_tex_font_outline_67->GetValue();
		this->font_outline_68 = map["font_outline_68"sv];
		this->_tex_font_outline_68 = this->font_outline_68->tex;
		this->_texid_font_outline_68 = this->_tex_font_outline_68->GetValue();
		this->font_outline_69 = map["font_outline_69"sv];
		this->_tex_font_outline_69 = this->font_outline_69->tex;
		this->_texid_font_outline_69 = this->_tex_font_outline_69->GetValue();
		this->font_outline_70 = map["font_outline_70"sv];
		this->_tex_font_outline_70 = this->font_outline_70->tex;
		this->_texid_font_outline_70 = this->_tex_font_outline_70->GetValue();
		this->font_outline_71 = map["font_outline_71"sv];
		this->_tex_font_outline_71 = this->font_outline_71->tex;
		this->_texid_font_outline_71 = this->_tex_font_outline_71->GetValue();
		this->font_outline_72 = map["font_outline_72"sv];
		this->_tex_font_outline_72 = this->font_outline_72->tex;
		this->_texid_font_outline_72 = this->_tex_font_outline_72->GetValue();
		this->font_outline_73 = map["font_outline_73"sv];
		this->_tex_font_outline_73 = this->font_outline_73->tex;
		this->_texid_font_outline_73 = this->_tex_font_outline_73->GetValue();
		this->font_outline_74 = map["font_outline_74"sv];
		this->_tex_font_outline_74 = this->font_outline_74->tex;
		this->_texid_font_outline_74 = this->_tex_font_outline_74->GetValue();
		this->font_outline_75 = map["font_outline_75"sv];
		this->_tex_font_outline_75 = this->font_outline_75->tex;
		this->_texid_font_outline_75 = this->_tex_font_outline_75->GetValue();
		this->font_outline_76 = map["font_outline_76"sv];
		this->_tex_font_outline_76 = this->font_outline_76->tex;
		this->_texid_font_outline_76 = this->_tex_font_outline_76->GetValue();
		this->font_outline_77 = map["font_outline_77"sv];
		this->_tex_font_outline_77 = this->font_outline_77->tex;
		this->_texid_font_outline_77 = this->_tex_font_outline_77->GetValue();
		this->font_outline_78 = map["font_outline_78"sv];
		this->_tex_font_outline_78 = this->font_outline_78->tex;
		this->_texid_font_outline_78 = this->_tex_font_outline_78->GetValue();
		this->font_outline_79 = map["font_outline_79"sv];
		this->_tex_font_outline_79 = this->font_outline_79->tex;
		this->_texid_font_outline_79 = this->_tex_font_outline_79->GetValue();
		this->font_outline_80 = map["font_outline_80"sv];
		this->_tex_font_outline_80 = this->font_outline_80->tex;
		this->_texid_font_outline_80 = this->_tex_font_outline_80->GetValue();
		this->font_outline_81 = map["font_outline_81"sv];
		this->_tex_font_outline_81 = this->font_outline_81->tex;
		this->_texid_font_outline_81 = this->_tex_font_outline_81->GetValue();
		this->font_outline_82 = map["font_outline_82"sv];
		this->_tex_font_outline_82 = this->font_outline_82->tex;
		this->_texid_font_outline_82 = this->_tex_font_outline_82->GetValue();
		this->font_outline_83 = map["font_outline_83"sv];
		this->_tex_font_outline_83 = this->font_outline_83->tex;
		this->_texid_font_outline_83 = this->_tex_font_outline_83->GetValue();
		this->font_outline_84 = map["font_outline_84"sv];
		this->_tex_font_outline_84 = this->font_outline_84->tex;
		this->_texid_font_outline_84 = this->_tex_font_outline_84->GetValue();
		this->font_outline_85 = map["font_outline_85"sv];
		this->_tex_font_outline_85 = this->font_outline_85->tex;
		this->_texid_font_outline_85 = this->_tex_font_outline_85->GetValue();
		this->font_outline_86 = map["font_outline_86"sv];
		this->_tex_font_outline_86 = this->font_outline_86->tex;
		this->_texid_font_outline_86 = this->_tex_font_outline_86->GetValue();
		this->font_outline_87 = map["font_outline_87"sv];
		this->_tex_font_outline_87 = this->font_outline_87->tex;
		this->_texid_font_outline_87 = this->_tex_font_outline_87->GetValue();
		this->font_outline_88 = map["font_outline_88"sv];
		this->_tex_font_outline_88 = this->font_outline_88->tex;
		this->_texid_font_outline_88 = this->_tex_font_outline_88->GetValue();
		this->font_outline_89 = map["font_outline_89"sv];
		this->_tex_font_outline_89 = this->font_outline_89->tex;
		this->_texid_font_outline_89 = this->_tex_font_outline_89->GetValue();
		this->font_outline_90 = map["font_outline_90"sv];
		this->_tex_font_outline_90 = this->font_outline_90->tex;
		this->_texid_font_outline_90 = this->_tex_font_outline_90->GetValue();
		this->font_outline_91 = map["font_outline_91"sv];
		this->_tex_font_outline_91 = this->font_outline_91->tex;
		this->_texid_font_outline_91 = this->_tex_font_outline_91->GetValue();
		this->font_outline_92 = map["font_outline_92"sv];
		this->_tex_font_outline_92 = this->font_outline_92->tex;
		this->_texid_font_outline_92 = this->_tex_font_outline_92->GetValue();
		this->font_outline_93 = map["font_outline_93"sv];
		this->_tex_font_outline_93 = this->font_outline_93->tex;
		this->_texid_font_outline_93 = this->_tex_font_outline_93->GetValue();
		this->font_outline_94 = map["font_outline_94"sv];
		this->_tex_font_outline_94 = this->font_outline_94->tex;
		this->_texid_font_outline_94 = this->_tex_font_outline_94->GetValue();
		this->font_outline_95 = map["font_outline_95"sv];
		this->_tex_font_outline_95 = this->font_outline_95->tex;
		this->_texid_font_outline_95 = this->_tex_font_outline_95->GetValue();
		this->font_outline_96 = map["font_outline_96"sv];
		this->_tex_font_outline_96 = this->font_outline_96->tex;
		this->_texid_font_outline_96 = this->_tex_font_outline_96->GetValue();
		this->font_outline_97 = map["font_outline_97"sv];
		this->_tex_font_outline_97 = this->font_outline_97->tex;
		this->_texid_font_outline_97 = this->_tex_font_outline_97->GetValue();
		this->font_outline_98 = map["font_outline_98"sv];
		this->_tex_font_outline_98 = this->font_outline_98->tex;
		this->_texid_font_outline_98 = this->_tex_font_outline_98->GetValue();
		this->font_outline_99 = map["font_outline_99"sv];
		this->_tex_font_outline_99 = this->font_outline_99->tex;
		this->_texid_font_outline_99 = this->_tex_font_outline_99->GetValue();
		this->font_outline_100 = map["font_outline_100"sv];
		this->_tex_font_outline_100 = this->font_outline_100->tex;
		this->_texid_font_outline_100 = this->_tex_font_outline_100->GetValue();
		this->font_outline_101 = map["font_outline_101"sv];
		this->_tex_font_outline_101 = this->font_outline_101->tex;
		this->_texid_font_outline_101 = this->_tex_font_outline_101->GetValue();
		this->font_outline_102 = map["font_outline_102"sv];
		this->_tex_font_outline_102 = this->font_outline_102->tex;
		this->_texid_font_outline_102 = this->_tex_font_outline_102->GetValue();
		this->font_outline_103 = map["font_outline_103"sv];
		this->_tex_font_outline_103 = this->font_outline_103->tex;
		this->_texid_font_outline_103 = this->_tex_font_outline_103->GetValue();
		this->font_outline_104 = map["font_outline_104"sv];
		this->_tex_font_outline_104 = this->font_outline_104->tex;
		this->_texid_font_outline_104 = this->_tex_font_outline_104->GetValue();
		this->font_outline_105 = map["font_outline_105"sv];
		this->_tex_font_outline_105 = this->font_outline_105->tex;
		this->_texid_font_outline_105 = this->_tex_font_outline_105->GetValue();
		this->font_outline_106 = map["font_outline_106"sv];
		this->_tex_font_outline_106 = this->font_outline_106->tex;
		this->_texid_font_outline_106 = this->_tex_font_outline_106->GetValue();
		this->font_outline_107 = map["font_outline_107"sv];
		this->_tex_font_outline_107 = this->font_outline_107->tex;
		this->_texid_font_outline_107 = this->_tex_font_outline_107->GetValue();
		this->font_outline_108 = map["font_outline_108"sv];
		this->_tex_font_outline_108 = this->font_outline_108->tex;
		this->_texid_font_outline_108 = this->_tex_font_outline_108->GetValue();
		this->font_outline_109 = map["font_outline_109"sv];
		this->_tex_font_outline_109 = this->font_outline_109->tex;
		this->_texid_font_outline_109 = this->_tex_font_outline_109->GetValue();
		this->font_outline_110 = map["font_outline_110"sv];
		this->_tex_font_outline_110 = this->font_outline_110->tex;
		this->_texid_font_outline_110 = this->_tex_font_outline_110->GetValue();
		this->font_outline_111 = map["font_outline_111"sv];
		this->_tex_font_outline_111 = this->font_outline_111->tex;
		this->_texid_font_outline_111 = this->_tex_font_outline_111->GetValue();
		this->font_outline_112 = map["font_outline_112"sv];
		this->_tex_font_outline_112 = this->font_outline_112->tex;
		this->_texid_font_outline_112 = this->_tex_font_outline_112->GetValue();
		this->font_outline_113 = map["font_outline_113"sv];
		this->_tex_font_outline_113 = this->font_outline_113->tex;
		this->_texid_font_outline_113 = this->_tex_font_outline_113->GetValue();
		this->font_outline_114 = map["font_outline_114"sv];
		this->_tex_font_outline_114 = this->font_outline_114->tex;
		this->_texid_font_outline_114 = this->_tex_font_outline_114->GetValue();
		this->font_outline_115 = map["font_outline_115"sv];
		this->_tex_font_outline_115 = this->font_outline_115->tex;
		this->_texid_font_outline_115 = this->_tex_font_outline_115->GetValue();
		this->font_outline_116 = map["font_outline_116"sv];
		this->_tex_font_outline_116 = this->font_outline_116->tex;
		this->_texid_font_outline_116 = this->_tex_font_outline_116->GetValue();
		this->font_outline_117 = map["font_outline_117"sv];
		this->_tex_font_outline_117 = this->font_outline_117->tex;
		this->_texid_font_outline_117 = this->_tex_font_outline_117->GetValue();
		this->font_outline_118 = map["font_outline_118"sv];
		this->_tex_font_outline_118 = this->font_outline_118->tex;
		this->_texid_font_outline_118 = this->_tex_font_outline_118->GetValue();
		this->font_outline_119 = map["font_outline_119"sv];
		this->_tex_font_outline_119 = this->font_outline_119->tex;
		this->_texid_font_outline_119 = this->_tex_font_outline_119->GetValue();
		this->font_outline_120 = map["font_outline_120"sv];
		this->_tex_font_outline_120 = this->font_outline_120->tex;
		this->_texid_font_outline_120 = this->_tex_font_outline_120->GetValue();
		this->font_outline_121 = map["font_outline_121"sv];
		this->_tex_font_outline_121 = this->font_outline_121->tex;
		this->_texid_font_outline_121 = this->_tex_font_outline_121->GetValue();
		this->font_outline_122 = map["font_outline_122"sv];
		this->_tex_font_outline_122 = this->font_outline_122->tex;
		this->_texid_font_outline_122 = this->_tex_font_outline_122->GetValue();
		this->font_outline_123 = map["font_outline_123"sv];
		this->_tex_font_outline_123 = this->font_outline_123->tex;
		this->_texid_font_outline_123 = this->_tex_font_outline_123->GetValue();
		this->font_outline_124 = map["font_outline_124"sv];
		this->_tex_font_outline_124 = this->font_outline_124->tex;
		this->_texid_font_outline_124 = this->_tex_font_outline_124->GetValue();
		this->font_outline_125 = map["font_outline_125"sv];
		this->_tex_font_outline_125 = this->font_outline_125->tex;
		this->_texid_font_outline_125 = this->_tex_font_outline_125->GetValue();
		this->font_outline_126 = map["font_outline_126"sv];
		this->_tex_font_outline_126 = this->font_outline_126->tex;
		this->_texid_font_outline_126 = this->_tex_font_outline_126->GetValue();
		this->ground_cell = map["ground_cell"sv];
		this->_tex_ground_cell = this->ground_cell->tex;
		this->_texid_ground_cell = this->_tex_ground_cell->GetValue();
		this->ground_cell2 = map["ground_cell2"sv];
		this->_tex_ground_cell2 = this->ground_cell2->tex;
		this->_texid_ground_cell2 = this->_tex_ground_cell2->GetValue();
		this->monster_chips = map["monster_chips"sv];
		this->_tex_monster_chips = this->monster_chips->tex;
		this->_texid_monster_chips = this->_tex_monster_chips->GetValue();
		this->monster_cola = map["monster_cola"sv];
		this->_tex_monster_cola = this->monster_cola->tex;
		this->_texid_monster_cola = this->_tex_monster_cola->GetValue();
		this->monster_hamburger = map["monster_hamburger"sv];
		this->_tex_monster_hamburger = this->monster_hamburger->tex;
		this->_texid_monster_hamburger = this->_tex_monster_hamburger->GetValue();
		this->monster_house = map["monster_house"sv];
		this->_tex_monster_house = this->monster_house->tex;
		this->_texid_monster_house = this->_tex_monster_house->GetValue();
		this->monster_instantnoodles = map["monster_instantnoodles"sv];
		this->_tex_monster_instantnoodles = this->monster_instantnoodles->tex;
		this->_texid_monster_instantnoodles = this->_tex_monster_instantnoodles->GetValue();
		this->monster_laptop = map["monster_laptop"sv];
		this->_tex_monster_laptop = this->monster_laptop->tex;
		this->_texid_monster_laptop = this->_tex_monster_laptop->GetValue();
		this->monster_roastduck = map["monster_roastduck"sv];
		this->_tex_monster_roastduck = this->monster_roastduck->tex;
		this->_texid_monster_roastduck = this->_tex_monster_roastduck->GetValue();
		this->monster_sofa = map["monster_sofa"sv];
		this->_tex_monster_sofa = this->monster_sofa->tex;
		this->_texid_monster_sofa = this->_tex_monster_sofa->GetValue();
		this->pixel = map["pixel"sv];
		this->_tex_pixel = this->pixel->tex;
		this->_texid_pixel = this->_tex_pixel->GetValue();
		this->player1 = map["player1"sv];
		this->_tex_player1 = this->player1->tex;
		this->_texid_player1 = this->_tex_player1->GetValue();
		this->player2 = map["player2"sv];
		this->_tex_player2 = this->player2->tex;
		this->_texid_player2 = this->_tex_player2->GetValue();
		this->ui_button = map["ui_button"sv];
		this->_tex_ui_button = this->ui_button->tex;
		this->_texid_ui_button = this->_tex_ui_button->GetValue();
		this->ui_hpbar = map["ui_hpbar"sv];
		this->_tex_ui_hpbar = this->ui_hpbar->tex;
		this->_texid_ui_hpbar = this->_tex_ui_hpbar->GetValue();
		this->ui_money = map["ui_money"sv];
		this->_tex_ui_money = this->ui_money->tex;
		this->_texid_ui_money = this->_tex_ui_money->GetValue();
		this->ui_paper = map["ui_paper"sv];
		this->_tex_ui_paper = this->ui_paper->tex;
		this->_texid_ui_paper = this->_tex_ui_paper->GetValue();
		this->ui_pause = map["ui_pause"sv];
		this->_tex_ui_pause = this->ui_pause->tex;
		this->_texid_ui_pause = this->_tex_ui_pause->GetValue();
		this->ui_refresh = map["ui_refresh"sv];
		this->_tex_ui_refresh = this->ui_refresh->tex;
		this->_texid_ui_refresh = this->_tex_ui_refresh->GetValue();
		this->ui_scrollbar = map["ui_scrollbar"sv];
		this->_tex_ui_scrollbar = this->ui_scrollbar->tex;
		this->_texid_ui_scrollbar = this->_tex_ui_scrollbar->GetValue();
		this->ui_scrollrect = map["ui_scrollrect"sv];
		this->_tex_ui_scrollrect = this->ui_scrollrect->tex;
		this->_texid_ui_scrollrect = this->_tex_ui_scrollrect->GetValue();
		this->warning1 = map["warning1"sv];
		this->_tex_warning1 = this->warning1->tex;
		this->_texid_warning1 = this->_tex_warning1->GetValue();
		this->warning1a = map["warning1a"sv];
		this->_tex_warning1a = this->warning1a->tex;
		this->_texid_warning1a = this->_tex_warning1a->GetValue();
		this->warning2 = map["warning2"sv];
		this->_tex_warning2 = this->warning2->tex;
		this->_texid_warning2 = this->_tex_warning2->GetValue();
	}

	// fill groups

	buff_.Add(buff_996);
	font_outline_.Add(font_outline_32);
	font_outline_.Add(font_outline_33);
	font_outline_.Add(font_outline_34);
	font_outline_.Add(font_outline_35);
	font_outline_.Add(font_outline_36);
	font_outline_.Add(font_outline_37);
	font_outline_.Add(font_outline_38);
	font_outline_.Add(font_outline_39);
	font_outline_.Add(font_outline_40);
	font_outline_.Add(font_outline_41);
	font_outline_.Add(font_outline_42);
	font_outline_.Add(font_outline_43);
	font_outline_.Add(font_outline_44);
	font_outline_.Add(font_outline_45);
	font_outline_.Add(font_outline_46);
	font_outline_.Add(font_outline_47);
	font_outline_.Add(font_outline_48);
	font_outline_.Add(font_outline_49);
	font_outline_.Add(font_outline_50);
	font_outline_.Add(font_outline_51);
	font_outline_.Add(font_outline_52);
	font_outline_.Add(font_outline_53);
	font_outline_.Add(font_outline_54);
	font_outline_.Add(font_outline_55);
	font_outline_.Add(font_outline_56);
	font_outline_.Add(font_outline_57);
	font_outline_.Add(font_outline_58);
	font_outline_.Add(font_outline_59);
	font_outline_.Add(font_outline_60);
	font_outline_.Add(font_outline_61);
	font_outline_.Add(font_outline_62);
	font_outline_.Add(font_outline_63);
	font_outline_.Add(font_outline_64);
	font_outline_.Add(font_outline_65);
	font_outline_.Add(font_outline_66);
	font_outline_.Add(font_outline_67);
	font_outline_.Add(font_outline_68);
	font_outline_.Add(font_outline_69);
	font_outline_.Add(font_outline_70);
	font_outline_.Add(font_outline_71);
	font_outline_.Add(font_outline_72);
	font_outline_.Add(font_outline_73);
	font_outline_.Add(font_outline_74);
	font_outline_.Add(font_outline_75);
	font_outline_.Add(font_outline_76);
	font_outline_.Add(font_outline_77);
	font_outline_.Add(font_outline_78);
	font_outline_.Add(font_outline_79);
	font_outline_.Add(font_outline_80);
	font_outline_.Add(font_outline_81);
	font_outline_.Add(font_outline_82);
	font_outline_.Add(font_outline_83);
	font_outline_.Add(font_outline_84);
	font_outline_.Add(font_outline_85);
	font_outline_.Add(font_outline_86);
	font_outline_.Add(font_outline_87);
	font_outline_.Add(font_outline_88);
	font_outline_.Add(font_outline_89);
	font_outline_.Add(font_outline_90);
	font_outline_.Add(font_outline_91);
	font_outline_.Add(font_outline_92);
	font_outline_.Add(font_outline_93);
	font_outline_.Add(font_outline_94);
	font_outline_.Add(font_outline_95);
	font_outline_.Add(font_outline_96);
	font_outline_.Add(font_outline_97);
	font_outline_.Add(font_outline_98);
	font_outline_.Add(font_outline_99);
	font_outline_.Add(font_outline_100);
	font_outline_.Add(font_outline_101);
	font_outline_.Add(font_outline_102);
	font_outline_.Add(font_outline_103);
	font_outline_.Add(font_outline_104);
	font_outline_.Add(font_outline_105);
	font_outline_.Add(font_outline_106);
	font_outline_.Add(font_outline_107);
	font_outline_.Add(font_outline_108);
	font_outline_.Add(font_outline_109);
	font_outline_.Add(font_outline_110);
	font_outline_.Add(font_outline_111);
	font_outline_.Add(font_outline_112);
	font_outline_.Add(font_outline_113);
	font_outline_.Add(font_outline_114);
	font_outline_.Add(font_outline_115);
	font_outline_.Add(font_outline_116);
	font_outline_.Add(font_outline_117);
	font_outline_.Add(font_outline_118);
	font_outline_.Add(font_outline_119);
	font_outline_.Add(font_outline_120);
	font_outline_.Add(font_outline_121);
	font_outline_.Add(font_outline_122);
	font_outline_.Add(font_outline_123);
	font_outline_.Add(font_outline_124);
	font_outline_.Add(font_outline_125);
	font_outline_.Add(font_outline_126);
	co_return;
}
